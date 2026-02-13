#include "../include/RW2Decoder.h"
#include "../include/RW2FrameDecode.h"

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

    // Basic validation - check for RW2 signature
    BYTE signature[16];
    ULONG bytesRead = 0;
    LARGE_INTEGER seekPos = { 0 };

    HRESULT hr = pIStream->Read(signature, sizeof(signature), &bytesRead);
    pIStream->Seek(seekPos, STREAM_SEEK_SET, nullptr);

    if (FAILED(hr) || bytesRead < 16)
        return WINCODEC_ERR_COMPONENTNOTFOUND;

    // RW2 files are TIFF-based, check for TIFF signature
    // II (little-endian) or MM (big-endian) followed by 0x002A or 0x0055 (for Panasonic)
    bool isValid = false;
    if ((signature[0] == 'I' && signature[1] == 'I' && signature[2] == 0x2A && signature[3] == 0x00) ||
        (signature[0] == 'I' && signature[1] == 'I' && signature[2] == 0x55 && signature[3] == 0x00) ||
        (signature[0] == 'M' && signature[1] == 'M' && signature[2] == 0x00 && signature[3] == 0x2A) ||
        (signature[0] == 'M' && signature[1] == 'M' && signature[2] == 0x00 && signature[3] == 0x55))
    {
        isValid = true;
    }

    if (!isValid)
        return WINCODEC_ERR_COMPONENTNOTFOUND;

    *pdwCapability = WICBitmapDecoderCapabilityCanDecodeAllImages;

    return S_OK;
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
    m_fileData.resize(fileSize);
    LARGE_INTEGER seekPos = { 0 };
    pIStream->Seek(seekPos, STREAM_SEEK_SET, nullptr);

    ULONG bytesRead = 0;
    hr = pIStream->Read(m_fileData.data(), static_cast<ULONG>(fileSize), &bytesRead);
    if (FAILED(hr) || bytesRead != fileSize)
    {
        m_fileData.clear();
        LeaveCriticalSection(&m_cs);
        return WINCODEC_ERR_BADIMAGE;
    }

    m_pStream = pIStream;
    m_pStream->AddRef();
    m_initialized = true;

    LeaveCriticalSection(&m_cs);
    return S_OK;
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
    return E_NOTIMPL; // Optional method
}

STDMETHODIMP RW2Decoder::CopyPalette(IWICPalette* pIPalette)
{
    return WINCODEC_ERR_PALETTEUNAVAILABLE;
}

STDMETHODIMP RW2Decoder::GetMetadataQueryReader(IWICMetadataQueryReader** ppIMetadataQueryReader)
{
    return E_NOTIMPL; // Could be implemented to expose EXIF data
}

STDMETHODIMP RW2Decoder::GetPreview(IWICBitmapSource** ppIBitmapSource)
{
    return WINCODEC_ERR_UNSUPPORTEDOPERATION;
}

STDMETHODIMP RW2Decoder::GetColorContexts(UINT cCount, IWICColorContext** ppIColorContexts, UINT* pcActualCount)
{
    if (pcActualCount == nullptr)
        return E_INVALIDARG;

    *pcActualCount = 0;
    return S_OK;
}

STDMETHODIMP RW2Decoder::GetThumbnail(IWICBitmapSource** ppIThumbnail)
{
    return WINCODEC_ERR_CODECNOTHUMBNAIL; // Could extract embedded JPEG thumbnail
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

    RW2FrameDecode* pFrame = new (std::nothrow) RW2FrameDecode(m_fileData.data(), m_fileData.size());
    if (pFrame == nullptr)
        return E_OUTOFMEMORY;

    HRESULT hr = pFrame->QueryInterface(IID_IWICBitmapFrameDecode, (void**)ppIBitmapFrame);
    pFrame->Release();

    return hr;
}
