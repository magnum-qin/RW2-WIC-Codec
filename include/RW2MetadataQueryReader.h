#pragma once

#include "Common.h"
#include <map>
#include <string>
#include <propvarutil.h>

// EXIF tag IDs used in WIC metadata queries
namespace ExifTag {
    constexpr USHORT ImageWidth          = 0x0100; // 256
    constexpr USHORT ImageHeight         = 0x0101; // 257
    constexpr USHORT Make                = 0x010F; // 271
    constexpr USHORT Model              = 0x0110; // 272
    constexpr USHORT Orientation        = 0x0112; // 274
    constexpr USHORT Software           = 0x0131; // 305
    constexpr USHORT DateTime           = 0x0132; // 306
    constexpr USHORT Artist             = 0x013B; // 315
    constexpr USHORT ExposureTime       = 0x829A; // 33434
    constexpr USHORT FNumber            = 0x829D; // 33437
    constexpr USHORT ISOSpeedRatings    = 0x8827; // 34855
    constexpr USHORT DateTimeOriginal   = 0x9003; // 36867
    constexpr USHORT DateTimeDigitized  = 0x9004; // 36868
    constexpr USHORT FocalLength        = 0x920A; // 37386
    constexpr USHORT PixelXDimension    = 0xA002; // 40962
    constexpr USHORT PixelYDimension    = 0xA003; // 40963
}

// Represents a single metadata value
struct MetadataValue
{
    enum Type { None, String, UShort, UInt, Double };
    Type type = None;
    std::wstring strVal;
    USHORT ushortVal = 0;
    UINT uintVal = 0;
    double doubleVal = 0.0;

    // Factory helpers
    static MetadataValue FromString(const wchar_t* s) {
        MetadataValue v; v.type = String; v.strVal = s; return v;
    }
    static MetadataValue FromString(const char* s) {
        MetadataValue v; v.type = String;
        if (s) {
            int len = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);
            v.strVal.resize(len - 1);
            MultiByteToWideChar(CP_UTF8, 0, s, -1, &v.strVal[0], len);
        }
        return v;
    }
    static MetadataValue FromUShort(USHORT val) {
        MetadataValue v; v.type = UShort; v.ushortVal = val; return v;
    }
    static MetadataValue FromUInt(UINT val) {
        MetadataValue v; v.type = UInt; v.uintVal = val; return v;
    }
    static MetadataValue FromDouble(double val) {
        MetadataValue v; v.type = Double; v.doubleVal = val; return v;
    }
};

class RW2MetadataQueryReader : public IWICMetadataQueryReader
{
public:
    RW2MetadataQueryReader(std::shared_ptr<std::vector<BYTE>> pData);
    virtual ~RW2MetadataQueryReader();

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // IWICMetadataQueryReader methods
    STDMETHOD(GetContainerFormat)(GUID* pguidContainerFormat) override;
    STDMETHOD(GetLocation)(UINT cchMaxLength, WCHAR* wzNamespace, UINT* pcchActualLength) override;
    STDMETHOD(GetMetadataByName)(LPCWSTR wzName, PROPVARIANT* pvarValue) override;
    STDMETHOD(GetEnumerator)(IEnumString** ppIEnumString) override;

private:
    void ExtractMetadata();
    static bool ParseTagId(LPCWSTR wzName, USHORT& tagId);
    HRESULT FillPropVariant(const MetadataValue& val, PROPVARIANT* pvar);

    LONG m_refCount;
    CRITICAL_SECTION m_cs;
    std::shared_ptr<std::vector<BYTE>> m_rawData;
    std::map<USHORT, MetadataValue> m_metadata;
    bool m_extracted;
};
