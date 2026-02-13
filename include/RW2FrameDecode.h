#pragma once

#include "Common.h"
#include <libraw/libraw.h>

class RW2FrameDecode : public IWICBitmapFrameDecode
{
public:
    RW2FrameDecode(const BYTE* pData, size_t dataSize);
    virtual ~RW2FrameDecode();

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // IWICBitmapSource methods
    STDMETHOD(GetSize)(UINT* puiWidth, UINT* puiHeight) override;
    STDMETHOD(GetPixelFormat)(WICPixelFormatGUID* pPixelFormat) override;
    STDMETHOD(GetResolution)(double* pDpiX, double* pDpiY) override;
    STDMETHOD(CopyPalette)(IWICPalette* pIPalette) override;
    STDMETHOD(CopyPixels)(const WICRect* prc, UINT cbStride, UINT cbBufferSize, BYTE* pbBuffer) override;

    // IWICBitmapFrameDecode methods
    STDMETHOD(GetMetadataQueryReader)(IWICMetadataQueryReader** ppIMetadataQueryReader) override;
    STDMETHOD(GetColorContexts)(UINT cCount, IWICColorContext** ppIColorContexts, UINT* pcActualCount) override;
    STDMETHOD(GetThumbnail)(IWICBitmapSource** ppIThumbnail) override;

private:
    HRESULT ProcessRawData();
    HRESULT EnsureProcessed();

    LONG m_refCount;
    CRITICAL_SECTION m_cs;

    std::vector<BYTE> m_rawData;
    std::unique_ptr<LibRaw> m_rawProcessor;
    libraw_processed_image_t* m_processedImage;

    UINT m_width;
    UINT m_height;
    bool m_processed;
    bool m_processingFailed;
};
