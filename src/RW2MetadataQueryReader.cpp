#include "../include/RW2MetadataQueryReader.h"
#include <libraw.h>
#include <ctime>
#include <cwchar>
#include <strsafe.h>

extern LONG g_objectCount;

RW2MetadataQueryReader::RW2MetadataQueryReader(std::shared_ptr<std::vector<BYTE>> pData)
    : m_refCount(1)
    , m_rawData(pData)
    , m_extracted(false)
{
    InitializeCriticalSection(&m_cs);
    InterlockedIncrement(&g_objectCount);
}

RW2MetadataQueryReader::~RW2MetadataQueryReader()
{
    DeleteCriticalSection(&m_cs);
    InterlockedDecrement(&g_objectCount);
}

// IUnknown

STDMETHODIMP RW2MetadataQueryReader::QueryInterface(REFIID riid, void** ppvObject)
{
    if (ppvObject == nullptr)
        return E_INVALIDARG;

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IWICMetadataQueryReader)
    {
        *ppvObject = static_cast<IWICMetadataQueryReader*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) RW2MetadataQueryReader::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) RW2MetadataQueryReader::Release()
{
    LONG refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0)
    {
        delete this;
    }
    return refCount;
}

// IWICMetadataQueryReader

STDMETHODIMP RW2MetadataQueryReader::GetContainerFormat(GUID* pguidContainerFormat)
{
    if (pguidContainerFormat == nullptr)
        return E_INVALIDARG;

    *pguidContainerFormat = GUID_ContainerFormatRaw;
    return S_OK;
}

STDMETHODIMP RW2MetadataQueryReader::GetLocation(UINT cchMaxLength, WCHAR* wzNamespace, UINT* pcchActualLength)
{
    if (pcchActualLength == nullptr)
        return E_INVALIDARG;

    // Root reader location is "/"
    const WCHAR location[] = L"/";
    *pcchActualLength = ARRAYSIZE(location); // includes null terminator

    if (wzNamespace != nullptr && cchMaxLength >= ARRAYSIZE(location))
    {
        StringCchCopyW(wzNamespace, cchMaxLength, location);
    }
    else if (wzNamespace != nullptr)
    {
        return WINCODEC_ERR_INSUFFICIENTBUFFER;
    }

    return S_OK;
}

STDMETHODIMP RW2MetadataQueryReader::GetMetadataByName(LPCWSTR wzName, PROPVARIANT* pvarValue)
{
    if (wzName == nullptr)
        return E_INVALIDARG;

    EnterCriticalSection(&m_cs);

    try
    {
        // Lazy extraction: parse metadata on first query
        if (!m_extracted)
        {
            ExtractMetadata();
            m_extracted = true;
        }

        // Parse the tag ID from the WIC metadata query path
        USHORT tagId = 0;
        if (!ParseTagId(wzName, tagId))
        {
            LeaveCriticalSection(&m_cs);
            return WINCODEC_ERR_PROPERTYNOTFOUND;
        }

        // Look up the tag
        auto it = m_metadata.find(tagId);
        if (it == m_metadata.end())
        {
            LeaveCriticalSection(&m_cs);
            return WINCODEC_ERR_PROPERTYNOTFOUND;
        }

        // If caller just wants to know if the property exists (pvarValue == nullptr)
        if (pvarValue == nullptr)
        {
            LeaveCriticalSection(&m_cs);
            return S_OK;
        }

        // Fill the PROPVARIANT
        PropVariantInit(pvarValue);
        HRESULT hr = FillPropVariant(it->second, pvarValue);
        LeaveCriticalSection(&m_cs);
        return hr;
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

STDMETHODIMP RW2MetadataQueryReader::GetEnumerator(IEnumString** ppIEnumString)
{
    // Not implemented - rarely called by Windows components
    if (ppIEnumString)
        *ppIEnumString = nullptr;
    return E_NOTIMPL;
}

// Private methods

void RW2MetadataQueryReader::ExtractMetadata()
{
    if (!m_rawData || m_rawData->empty())
        return;

    LibRaw rawProcessor;

    // open_buffer only reads headers and metadata, not the full raw data
    int ret = rawProcessor.open_buffer(m_rawData->data(), m_rawData->size());
    if (ret != LIBRAW_SUCCESS)
        return;

    const auto& idata = rawProcessor.imgdata.idata;
    const auto& other = rawProcessor.imgdata.other;
    const auto& sizes = rawProcessor.imgdata.sizes;

    // Camera make and model
    if (idata.make[0] != '\0')
        m_metadata[ExifTag::Make] = MetadataValue::FromString(idata.make);

    if (idata.model[0] != '\0')
        m_metadata[ExifTag::Model] = MetadataValue::FromString(idata.model);

    // Image dimensions
    if (sizes.width > 0)
    {
        m_metadata[ExifTag::ImageWidth] = MetadataValue::FromUInt(sizes.width);
        m_metadata[ExifTag::PixelXDimension] = MetadataValue::FromUInt(sizes.width);
    }
    if (sizes.height > 0)
    {
        m_metadata[ExifTag::ImageHeight] = MetadataValue::FromUInt(sizes.height);
        m_metadata[ExifTag::PixelYDimension] = MetadataValue::FromUInt(sizes.height);
    }

    // Orientation (LibRaw flip values: 0=none, 3=180, 5=270cw, 6=90cw)
    // Map to EXIF orientation values
    {
        USHORT orientation = 1; // default: top-left
        switch (sizes.flip)
        {
        case 0:   orientation = 1; break; // Normal
        case 3:   orientation = 3; break; // 180°
        case 5:   orientation = 8; break; // 270° CW = 90° CCW
        case 6:   orientation = 6; break; // 90° CW
        default:  orientation = 1; break;
        }
        m_metadata[ExifTag::Orientation] = MetadataValue::FromUShort(orientation);
    }

    // Exposure time (shutter speed in seconds)
    if (other.shutter > 0.0f)
    {
        m_metadata[ExifTag::ExposureTime] = MetadataValue::FromDouble(static_cast<double>(other.shutter));
    }

    // Aperture (f-number)
    if (other.aperture > 0.0f)
    {
        m_metadata[ExifTag::FNumber] = MetadataValue::FromDouble(static_cast<double>(other.aperture));
    }

    // ISO speed
    if (other.iso_speed > 0.0f)
    {
        if (other.iso_speed > 65535.0f)
        {
            m_metadata[ExifTag::ISOSpeedRatings] = MetadataValue::FromUInt(
                static_cast<UINT>(other.iso_speed));
        }
        else
        {
            m_metadata[ExifTag::ISOSpeedRatings] = MetadataValue::FromUShort(
                static_cast<USHORT>(other.iso_speed));
        }
    }

    // Focal length (mm)
    if (other.focal_len > 0.0f)
    {
        m_metadata[ExifTag::FocalLength] = MetadataValue::FromDouble(static_cast<double>(other.focal_len));
    }

    // Date/time from timestamp
    if (other.timestamp != 0)
    {
        struct tm timeinfo;
        localtime_s(&timeinfo, &other.timestamp);

        WCHAR dateStr[32];
        StringCchPrintfW(dateStr, ARRAYSIZE(dateStr),
            L"%04d:%02d:%02d %02d:%02d:%02d",
            timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

        m_metadata[ExifTag::DateTime] = MetadataValue::FromString(dateStr);
        m_metadata[ExifTag::DateTimeOriginal] = MetadataValue::FromString(dateStr);
        m_metadata[ExifTag::DateTimeDigitized] = MetadataValue::FromString(dateStr);
    }

    // Artist / description
    if (other.artist[0] != '\0')
        m_metadata[ExifTag::Artist] = MetadataValue::FromString(other.artist);

    if (other.desc[0] != '\0')
        m_metadata[ExifTag::Software] = MetadataValue::FromString(other.desc);
}

bool RW2MetadataQueryReader::ParseTagId(LPCWSTR wzName, USHORT& tagId)
{
    // Parse WIC metadata query paths to extract the EXIF tag ID.
    // Supported formats:
    //   /{ushort=NNN}
    //   /ifd/{ushort=NNN}
    //   /ifd/exif/{ushort=NNN}
    //   /app1/ifd/{ushort=NNN}
    //   /app1/ifd/exif/{ushort=NNN}
    //   /ifd/exif/subifd:{ushort=NNN}
    //
    // We extract the numeric value from the last {ushort=NNN} token.

    if (wzName == nullptr)
        return false;

    // Find the last occurrence of "{ushort=" in the query string
    const WCHAR* pattern = L"{ushort=";
    const size_t patternLen = wcslen(pattern);

    const WCHAR* lastMatch = nullptr;
    const WCHAR* p = wzName;
    while (*p)
    {
        // Case-insensitive comparison
        if (_wcsnicmp(p, pattern, patternLen) == 0)
        {
            lastMatch = p;
        }
        p++;
    }

    if (lastMatch == nullptr)
        return false;

    // Extract the number after "{ushort="
    const WCHAR* numStart = lastMatch + patternLen;
    WCHAR* numEnd = nullptr;
    unsigned long val = wcstoul(numStart, &numEnd, 10);

    // Must end with '}'
    if (numEnd == nullptr || *numEnd != L'}')
        return false;

    if (val > 0xFFFF)
        return false;

    tagId = static_cast<USHORT>(val);
    return true;
}

HRESULT RW2MetadataQueryReader::FillPropVariant(const MetadataValue& val, PROPVARIANT* pvar)
{
    switch (val.type)
    {
    case MetadataValue::String:
    {
        pvar->vt = VT_LPWSTR;
        size_t len = val.strVal.length() + 1;
        pvar->pwszVal = static_cast<LPWSTR>(CoTaskMemAlloc(len * sizeof(WCHAR)));
        if (pvar->pwszVal == nullptr)
            return E_OUTOFMEMORY;
        StringCchCopyW(pvar->pwszVal, len, val.strVal.c_str());
        return S_OK;
    }

    case MetadataValue::UShort:
        pvar->vt = VT_UI2;
        pvar->uiVal = val.ushortVal;
        return S_OK;

    case MetadataValue::UInt:
        pvar->vt = VT_UI4;
        pvar->ulVal = val.uintVal;
        return S_OK;

    case MetadataValue::Double:
        pvar->vt = VT_R8;
        pvar->dblVal = val.doubleVal;
        return S_OK;

    default:
        return WINCODEC_ERR_PROPERTYNOTFOUND;
    }
}
