#pragma once

#include "Common.h"

class RW2Decoder : public IWICBitmapDecoder
{
public:
    RW2Decoder();
    virtual ~RW2Decoder();

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // IWICBitmapDecoder methods
    STDMETHOD(QueryCapability)(IStream* pIStream, DWORD* pdwCapability) override;
    STDMETHOD(Initialize)(IStream* pIStream, WICDecodeOptions cacheOptions) override;
    STDMETHOD(GetContainerFormat)(GUID* pguidContainerFormat) override;
    STDMETHOD(GetDecoderInfo)(IWICBitmapDecoderInfo** ppIDecoderInfo) override;
    STDMETHOD(CopyPalette)(IWICPalette* pIPalette) override;
    STDMETHOD(GetMetadataQueryReader)(IWICMetadataQueryReader** ppIMetadataQueryReader) override;
    STDMETHOD(GetPreview)(IWICBitmapSource** ppIBitmapSource) override;
    STDMETHOD(GetColorContexts)(UINT cCount, IWICColorContext** ppIColorContexts, UINT* pcActualCount) override;
    STDMETHOD(GetThumbnail)(IWICBitmapSource** ppIThumbnail) override;
    STDMETHOD(GetFrameCount)(UINT* pCount) override;
    STDMETHOD(GetFrame)(UINT index, IWICBitmapFrameDecode** ppIBitmapFrame) override;

private:
    LONG m_refCount;
    IStream* m_pStream;
    CRITICAL_SECTION m_cs;
    bool m_initialized;
    std::vector<BYTE> m_fileData;
};
