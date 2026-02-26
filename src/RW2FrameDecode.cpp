#include "../include/RW2FrameDecode.h"

extern LONG g_objectCount;

RW2FrameDecode::RW2FrameDecode(const BYTE* pData, size_t dataSize)
    : m_refCount(1)
    , m_processedImage(nullptr)
    , m_width(0)
    , m_height(0)
    , m_processed(false)
    , m_processingFailed(false)
{
    InitializeCriticalSection(&m_cs);
    InterlockedIncrement(&g_objectCount);

    if (pData && dataSize > 0)
    {
        m_rawData.assign(pData, pData + dataSize);
        m_rawProcessor = std::make_unique<LibRaw>();
    }
}

RW2FrameDecode::~RW2FrameDecode()
{
    if (m_processedImage)
    {
        LibRaw::dcraw_clear_mem(m_processedImage);
        m_processedImage = nullptr;
    }

    DeleteCriticalSection(&m_cs);
    InterlockedDecrement(&g_objectCount);
}

STDMETHODIMP RW2FrameDecode::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr)
        return E_INVALIDARG;

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IWICBitmapSource || riid == IID_IWICBitmapFrameDecode)
    {
        *ppvObject = static_cast<IWICBitmapFrameDecode*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) RW2FrameDecode::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) RW2FrameDecode::Release()
{
    LONG refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0)
    {
        delete this;
    }
    return refCount;
}

HRESULT RW2FrameDecode::ProcessRawData()
{
    if (!m_rawProcessor || m_rawData.empty())
        return E_FAIL;

    int ret = m_rawProcessor->open_buffer(m_rawData.data(), m_rawData.size());
    if (ret != LIBRAW_SUCCESS)
        return WINCODEC_ERR_BADIMAGE;

    // Get image dimensions before processing
    m_width = m_rawProcessor->imgdata.sizes.width;
    m_height = m_rawProcessor->imgdata.sizes.height;

    // Unpack the raw data
    ret = m_rawProcessor->unpack();
    if (ret != LIBRAW_SUCCESS)
        return WINCODEC_ERR_BADIMAGE;

    // Configure processing parameters
    m_rawProcessor->imgdata.params.use_camera_wb = 1;      // Use camera white balance
    m_rawProcessor->imgdata.params.output_color = 1;       // sRGB color space
    m_rawProcessor->imgdata.params.output_bps = 8;         // 8-bit output
    m_rawProcessor->imgdata.params.user_qual = 3;          // AHD interpolation (high quality)
    m_rawProcessor->imgdata.params.no_auto_bright = 0;     // Enable auto brightness
    m_rawProcessor->imgdata.params.use_fuji_rotate = 1;    // Auto-rotate

    // Process the raw data
    ret = m_rawProcessor->dcraw_process();
    if (ret != LIBRAW_SUCCESS)
        return WINCODEC_ERR_BADIMAGE;

    // Create processed image in memory
    m_processedImage = m_rawProcessor->dcraw_make_mem_image(&ret);
    if (!m_processedImage || ret != LIBRAW_SUCCESS)
        return WINCODEC_ERR_BADIMAGE;

    // Update dimensions from processed image
    m_width = m_processedImage->width;
    m_height = m_processedImage->height;

    return S_OK;
}

HRESULT RW2FrameDecode::EnsureProcessed()
{
    EnterCriticalSection(&m_cs);

    if (m_processed)
    {
        LeaveCriticalSection(&m_cs);
        return S_OK;
    }

    if (m_processingFailed)
    {
        LeaveCriticalSection(&m_cs);
        return WINCODEC_ERR_BADIMAGE;
    }

    HRESULT hr = ProcessRawData();
    if (SUCCEEDED(hr))
    {
        m_processed = true;
    }
    else
    {
        m_processingFailed = true;
    }

    LeaveCriticalSection(&m_cs);
    return hr;
}

STDMETHODIMP RW2FrameDecode::GetSize(UINT* puiWidth, UINT* puiHeight)
{
    if (puiWidth == nullptr || puiHeight == nullptr)
        return E_INVALIDARG;

    HRESULT hr = EnsureProcessed();
    if (FAILED(hr))
        return hr;

    *puiWidth = m_width;
    *puiHeight = m_height;

    return S_OK;
}

STDMETHODIMP RW2FrameDecode::GetPixelFormat(WICPixelFormatGUID* pPixelFormat)
{
    if (pPixelFormat == nullptr)
        return E_INVALIDARG;

    // LibRaw outputs RGB, but WIC consumers expect BGR memory layout.
    // We report BGR and swap R/B channels in CopyPixels.
    *pPixelFormat = GUID_WICPixelFormat24bppBGR;
    return S_OK;
}

STDMETHODIMP RW2FrameDecode::GetResolution(double* pDpiX, double* pDpiY)
{
    if (pDpiX == nullptr || pDpiY == nullptr)
        return E_INVALIDARG;

    // Default to 72 DPI (could be extracted from EXIF)
    *pDpiX = 72.0;
    *pDpiY = 72.0;

    return S_OK;
}

STDMETHODIMP RW2FrameDecode::CopyPalette(IWICPalette* pIPalette)
{
    return WINCODEC_ERR_PALETTEUNAVAILABLE;
}

STDMETHODIMP RW2FrameDecode::CopyPixels(const WICRect* prc, UINT cbStride, UINT cbBufferSize, BYTE* pbBuffer)
{
    if (pbBuffer == nullptr)
        return E_INVALIDARG;

    HRESULT hr = EnsureProcessed();
    if (FAILED(hr))
        return hr;

    if (!m_processedImage)
        return E_FAIL;

    // Validate processed image format
    if (m_processedImage->type != LIBRAW_IMAGE_BITMAP)
        return WINCODEC_ERR_UNSUPPORTEDPIXELFORMAT;

    // LibRaw outputs RGB data
    UINT bytesPerPixel = 3; // 24bpp RGB
    UINT imageStride = m_width * bytesPerPixel;

    // Determine copy region
    WICRect copyRect;
    if (prc == nullptr)
    {
        copyRect.X = 0;
        copyRect.Y = 0;
        copyRect.Width = m_width;
        copyRect.Height = m_height;
    }
    else
    {
        copyRect = *prc;
    }

    // Validate rectangle
    if (copyRect.X < 0 || copyRect.Y < 0 ||
        copyRect.X + copyRect.Width > static_cast<INT>(m_width) ||
        copyRect.Y + copyRect.Height > static_cast<INT>(m_height))
    {
        return E_INVALIDARG;
    }

    // Validate stride
    UINT minStride = copyRect.Width * bytesPerPixel;
    if (cbStride < minStride)
        return E_INVALIDARG;

    // Validate buffer size (use 64-bit to prevent overflow)
    ULONGLONG requiredSize = (ULONGLONG)cbStride * copyRect.Height;
    if (cbBufferSize < requiredSize)
        return E_INVALIDARG;

    // Copy pixel data, converting LibRaw RGB -> WIC BGR
    BYTE* pSrc = m_processedImage->data + (copyRect.Y * imageStride) + (copyRect.X * bytesPerPixel);
    BYTE* pDst = pbBuffer;

    for (INT row = 0; row < copyRect.Height; ++row)
    {
        // Swap R and B channels: LibRaw gives R,G,B but WIC 24bppBGR expects B,G,R
        for (INT col = 0; col < copyRect.Width; ++col)
        {
            BYTE* srcPixel = pSrc + col * bytesPerPixel;
            BYTE* dstPixel = pDst + col * bytesPerPixel;
            dstPixel[0] = srcPixel[2]; // B <- R
            dstPixel[1] = srcPixel[1]; // G <- G
            dstPixel[2] = srcPixel[0]; // R <- B
        }
        pSrc += imageStride;
        pDst += cbStride;
    }

    return S_OK;
}

STDMETHODIMP RW2FrameDecode::GetMetadataQueryReader(IWICMetadataQueryReader** ppIMetadataQueryReader)
{
    return E_NOTIMPL; // Could be implemented to expose EXIF metadata
}

STDMETHODIMP RW2FrameDecode::GetColorContexts(UINT cCount, IWICColorContext** ppIColorContexts, UINT* pcActualCount)
{
    if (pcActualCount == nullptr)
        return E_INVALIDARG;

    *pcActualCount = 0;
    return S_OK;
}

STDMETHODIMP RW2FrameDecode::GetThumbnail(IWICBitmapSource** ppIThumbnail)
{
    return WINCODEC_ERR_CODECNOTHUMBNAIL;
}
