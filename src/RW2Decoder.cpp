#include "../include/RW2Decoder.h"
#include "../include/RW2FrameDecode.h"
#include "../include/RW2MetadataQueryReader.h"

extern LONG g_objectCount;

RW2Decoder::RW2Decoder()
    : m_refCount(1)
    , m_pStream(nullptr)
    , m_initialized(false)
{
    InitializeCriticalSection(&m_cs);
    InterlockedIncrement(&g_objectCount);
}

RW2Decoder::~RW2Decoder()
{
    SAFE_RELEASE(m_pStream);
    DeleteCriticalSection(&m_cs);
    InterlockedDecrement(&g_objectCount);
}

STDMETHODIMP RW2Decoder::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr)
        return E_INVALIDARG;

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IWICBitmapDecoder)
    {
        *ppvObject = static_cast<IWICBitmapDecoder*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) RW2Decoder::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) RW2Decoder::Release()
{
    LONG refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0)
    {
        delete this;
    }
    return refCount;
}

STDMETHODIMP RW2Decoder::QueryCapability(IStream* pIStream, DWORD* pdwCapability)
{
    if (pIStream == nullptr || pdwCapability == nullptr)
        return E_INVALIDARG;

    try
    {
        // Basic validation - check for RW2 signature
        BYTE signature[16];
        ULONG bytesRead = 0;
        LARGE_INTEGER seekPos = { 0 };

        HRESULT hr = pIStream->Read(signature, sizeof(signature), &bytesRead);
        pIStream->Seek(seekPos, STREAM_SEEK_SET, nullptr);

        if (FAILED(hr) || bytesRead < 16)
            return WINCODEC_ERR_COMPONENTNOTFOUND;

        // RW2 files are TIFF-based, check for Panasonic TIFF signature
        // II (little-endian) or MM (big-endian) followed by 0x0055 (for Panasonic)
        bool isValid = false;
        if ((signature[0] == 'I' && signature[1] == 'I' && signature[2] == 0x55 && signature[3] == 0x00) ||
            (signature[0] == 'M' && signature[1] == 'M' && signature[2] == 0x00 && signature[3] == 0x55))
        {
            isValid = true;
        }

        if (!isValid)
            return WINCODEC_ERR_COMPONENTNOTFOUND;

        *pdwCapability = WICBitmapDecoderCapabilityCanDecodeAllImages;

        return S_OK;
    }
    catch (const std::bad_alloc&)
    {
        return E_OUTOFMEMORY;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

STDMETHODIMP RW2Decoder::Initialize(IStream* pIStream, WICDecodeOptions cacheOptions)
{
    if (pIStream == nullptr)
        return E_INVALIDARG;

    EnterCriticalSection(&m_cs);

    if (m_initialized)
    {
        LeaveCriticalSection(&m_cs);
        return WINCODEC_ERR_WRONGSTATE;
    }

    HRESULT hr = S_OK;

    try
    {
        // Ensure dependent DLLs are loaded
        hr = EnsureDependenciesLoaded();
        if (FAILED(hr))
        {
            LeaveCriticalSection(&m_cs);
            return hr;
        }

        // Get stream size
        STATSTG stat;
        hr = pIStream->Stat(&stat, STATFLAG_NONAME);
        if (FAILED(hr))
        {
            LeaveCriticalSection(&m_cs);
            return hr;
        }

        size_t fileSize = static_cast<size_t>(stat.cbSize.QuadPart);
        if (fileSize == 0 || fileSize > 500 * 1024 * 1024) // Max 500MB
        {
            LeaveCriticalSection(&m_cs);
            return WINCODEC_ERR_BADIMAGE;
        }

        // Read entire stream into memory
        m_fileData = std::make_shared<std::vector<BYTE>>(fileSize);
        LARGE_INTEGER seekPos = { 0 };
        pIStream->Seek(seekPos, STREAM_SEEK_SET, nullptr);

        ULONG bytesRead = 0;
        hr = pIStream->Read(m_fileData->data(), static_cast<ULONG>(fileSize), &bytesRead);
        if (FAILED(hr) || bytesRead != fileSize)
        {
            m_fileData.reset();
            LeaveCriticalSection(&m_cs);
            return WINCODEC_ERR_BADIMAGE;
        }

        m_pStream = pIStream;
        m_pStream->AddRef();
        m_initialized = true;

        LeaveCriticalSection(&m_cs);
        return S_OK;
    }
    catch (const std::bad_alloc&)
    {
        LeaveCriticalSection(&m_cs);
        return E_OUTOFMEMORY;
    }
    catch (...)
    {
        LeaveCriticalSection(&m_cs);
        return E_FAIL;
    }
}

STDMETHODIMP RW2Decoder::GetContainerFormat(GUID* pguidContainerFormat)
{
    if (pguidContainerFormat == nullptr)
        return E_INVALIDARG;

    *pguidContainerFormat = GUID_ContainerFormatRaw;
    return S_OK;
}

STDMETHODIMP RW2Decoder::GetDecoderInfo(IWICBitmapDecoderInfo** ppIDecoderInfo)
{
    if (ppIDecoderInfo == nullptr)
        return E_INVALIDARG;

    *ppIDecoderInfo = nullptr;

    try
    {
        IWICImagingFactory* pFactory = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
            CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
        if (SUCCEEDED(hr))
        {
            IWICComponentInfo* pCompInfo = nullptr;
            hr = pFactory->CreateComponentInfo(CLSID_RW2Decoder, &pCompInfo);
            if (SUCCEEDED(hr))
            {
                hr = pCompInfo->QueryInterface(IID_IWICBitmapDecoderInfo,
                    (void**)ppIDecoderInfo);
                pCompInfo->Release();
            }
            pFactory->Release();
        }
        return hr;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

STDMETHODIMP RW2Decoder::CopyPalette(IWICPalette* pIPalette)
{
    return WINCODEC_ERR_PALETTEUNAVAILABLE;
}

STDMETHODIMP RW2Decoder::GetMetadataQueryReader(IWICMetadataQueryReader** ppIMetadataQueryReader)
{
    if (ppIMetadataQueryReader == nullptr)
        return E_INVALIDARG;

    *ppIMetadataQueryReader = nullptr;

    if (!m_initialized)
        return WINCODEC_ERR_NOTINITIALIZED;

    try
    {
        RW2MetadataQueryReader* pReader = new (std::nothrow) RW2MetadataQueryReader(m_fileData);
        if (pReader == nullptr)
            return E_OUTOFMEMORY;

        HRESULT hr = pReader->QueryInterface(IID_IWICMetadataQueryReader,
            (void**)ppIMetadataQueryReader);
        pReader->Release();

        return hr;
    }
    catch (const std::bad_alloc&)
    {
        return E_OUTOFMEMORY;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

STDMETHODIMP RW2Decoder::GetPreview(IWICBitmapSource** ppIBitmapSource)
{
    if (ppIBitmapSource == nullptr)
        return E_INVALIDARG;

    *ppIBitmapSource = nullptr;

    if (!m_initialized)
        return WINCODEC_ERR_NOTINITIALIZED;

    try
    {
        HRESULT hr = EnsureDependenciesLoaded();
        if (FAILED(hr))
            return hr;

        // RW2 files contain an embedded JPEG preview; use it as the preview image
        LibRaw previewRaw;
        int ret = previewRaw.open_buffer(m_fileData->data(), m_fileData->size());
        if (ret != LIBRAW_SUCCESS)
            return WINCODEC_ERR_BADIMAGE;

        ret = previewRaw.unpack_thumb();
        if (ret != LIBRAW_SUCCESS)
            return WINCODEC_ERR_UNSUPPORTEDOPERATION;

        if (previewRaw.imgdata.thumbnail.tformat != LIBRAW_THUMBNAIL_JPEG ||
            previewRaw.imgdata.thumbnail.tlength == 0 ||
            previewRaw.imgdata.thumbnail.thumb == nullptr)
        {
            return WINCODEC_ERR_UNSUPPORTEDOPERATION;
        }

        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, previewRaw.imgdata.thumbnail.tlength);
        if (!hGlobal)
            return E_OUTOFMEMORY;

        void* pData = GlobalLock(hGlobal);
        if (pData)
        {
            memcpy(pData, previewRaw.imgdata.thumbnail.thumb, previewRaw.imgdata.thumbnail.tlength);
            GlobalUnlock(hGlobal);
        }
        else
        {
            GlobalFree(hGlobal);
            return E_OUTOFMEMORY;
        }

        IStream* pStream = nullptr;
        hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
        if (FAILED(hr))
        {
            GlobalFree(hGlobal);
            return hr;
        }

        IWICImagingFactory* pFactory = nullptr;
        hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
            CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
        if (SUCCEEDED(hr))
        {
            IWICBitmapDecoder* pJpegDecoder = nullptr;
            hr = pFactory->CreateDecoderFromStream(pStream, nullptr,
                WICDecodeMetadataCacheOnDemand, &pJpegDecoder);
            if (SUCCEEDED(hr))
            {
                IWICBitmapFrameDecode* pFrame = nullptr;
                hr = pJpegDecoder->GetFrame(0, &pFrame);
                if (SUCCEEDED(hr))
                {
                    hr = pFrame->QueryInterface(IID_IWICBitmapSource, (void**)ppIBitmapSource);
                    pFrame->Release();
                }
                pJpegDecoder->Release();
            }
            pFactory->Release();
        }

        pStream->Release();
        return hr;
    }
    catch (const std::bad_alloc&)
    {
        return E_OUTOFMEMORY;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

STDMETHODIMP RW2Decoder::GetColorContexts(UINT cCount, IWICColorContext** ppIColorContexts, UINT* pcActualCount)
{
    if (pcActualCount == nullptr)
        return E_INVALIDARG;

    *pcActualCount = 1; // We provide one sRGB color context

    if (cCount == 0 || ppIColorContexts == nullptr)
        return S_OK; // Caller is just querying the count

    try
    {
        IWICImagingFactory* pFactory = nullptr;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr,
            CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
        if (SUCCEEDED(hr))
        {
            hr = pFactory->CreateColorContext(&ppIColorContexts[0]);
            if (SUCCEEDED(hr))
            {
                hr = ppIColorContexts[0]->InitializeFromExifColorSpace(1); // 1 = sRGB
            }
            pFactory->Release();
        }
        return hr;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

STDMETHODIMP RW2Decoder::GetThumbnail(IWICBitmapSource** ppIThumbnail)
{
    if (ppIThumbnail == nullptr)
        return E_INVALIDARG;

    *ppIThumbnail = nullptr;

    if (!m_initialized)
        return WINCODEC_ERR_NOTINITIALIZED;

    try
    {
        HRESULT hr = EnsureDependenciesLoaded();
        if (FAILED(hr))
            return hr;

        // Use a local LibRaw instance just to grab the thumbnail quickly
        LibRaw thumbRaw;
        int ret = thumbRaw.open_buffer(m_fileData->data(), m_fileData->size());
        if (ret != LIBRAW_SUCCESS)
            return WINCODEC_ERR_BADIMAGE;

        ret = thumbRaw.unpack_thumb();
        if (ret != LIBRAW_SUCCESS)
            return WINCODEC_ERR_CODECNOTHUMBNAIL;

        // We only handle embedded JPEG thumbnails
        if (thumbRaw.imgdata.thumbnail.tformat != LIBRAW_THUMBNAIL_JPEG ||
            thumbRaw.imgdata.thumbnail.tlength == 0 ||
            thumbRaw.imgdata.thumbnail.thumb == nullptr)
        {
            return WINCODEC_ERR_CODECNOTHUMBNAIL;
        }

        // Allocate memory for the JPEG stream
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, thumbRaw.imgdata.thumbnail.tlength);
        if (!hGlobal)
            return E_OUTOFMEMORY;

        void* pData = GlobalLock(hGlobal);
        if (pData)
        {
            memcpy(pData, thumbRaw.imgdata.thumbnail.thumb, thumbRaw.imgdata.thumbnail.tlength);
            GlobalUnlock(hGlobal);
        }
        else
        {
            GlobalFree(hGlobal);
            return E_OUTOFMEMORY;
        }

        // Create an IStream over the global memory
        IStream* pStream = nullptr;
        hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream); // TRUE means stream frees hGlobal on release
        if (FAILED(hr))
        {
            GlobalFree(hGlobal);
            return hr;
        }

        // Create a generic WIC Factory to decode the JPEG stream
        IWICImagingFactory* pFactory = nullptr;
        hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));
        if (SUCCEEDED(hr))
        {
            IWICBitmapDecoder* pJpegDecoder = nullptr;
            // Ask WIC to automatically deduce it's a JPEG and create a decoder
            hr = pFactory->CreateDecoderFromStream(pStream, nullptr, WICDecodeMetadataCacheOnDemand, &pJpegDecoder);
            if (SUCCEEDED(hr))
            {
                IWICBitmapFrameDecode* pFrame = nullptr;
                hr = pJpegDecoder->GetFrame(0, &pFrame);
                if (SUCCEEDED(hr))
                {
                    // The frame itself implements IWICBitmapSource
                    hr = pFrame->QueryInterface(IID_IWICBitmapSource, (void**)ppIThumbnail);
                    pFrame->Release();
                }
                pJpegDecoder->Release();
            }
            pFactory->Release();
        }

        pStream->Release();
        return hr;
    }
    catch (const std::bad_alloc&)
    {
        return E_OUTOFMEMORY;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

STDMETHODIMP RW2Decoder::GetFrameCount(UINT* pCount)
{
    if (pCount == nullptr)
        return E_INVALIDARG;

    if (!m_initialized)
        return WINCODEC_ERR_NOTINITIALIZED;

    *pCount = 1; // RW2 files have single frame
    return S_OK;
}

STDMETHODIMP RW2Decoder::GetFrame(UINT index, IWICBitmapFrameDecode** ppIBitmapFrame)
{
    if (ppIBitmapFrame == nullptr)
        return E_INVALIDARG;

    *ppIBitmapFrame = nullptr;

    if (!m_initialized)
        return WINCODEC_ERR_NOTINITIALIZED;

    if (index != 0)
        return E_INVALIDARG;

    try
    {
        RW2FrameDecode* pFrame = new (std::nothrow) RW2FrameDecode(m_fileData);
        if (pFrame == nullptr)
            return E_OUTOFMEMORY;

        HRESULT hr = pFrame->QueryInterface(IID_IWICBitmapFrameDecode, (void**)ppIBitmapFrame);
        pFrame->Release();

        return hr;
    }
    catch (const std::bad_alloc&)
    {
        return E_OUTOFMEMORY;
    }
    catch (...)
    {
        return E_FAIL;
    }
}
