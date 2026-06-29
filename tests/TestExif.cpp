#include <windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <iostream>
#include <locale>
#include <propvarutil.h>

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "propsys.lib")
#pragma comment(lib, "ole32.lib")

void QueryAndPrint(IWICMetadataQueryReader* pReader, LPCWSTR name, LPCWSTR label)
{
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = pReader->GetMetadataByName(name, &var);
    if (SUCCEEDED(hr))
    {
        std::wcout << L"  " << label << L": ";
        switch (var.vt)
        {
        case VT_LPWSTR: std::wcout << var.pwszVal; break;
        case VT_UI2:    std::wcout << var.uiVal; break;
        case VT_UI4:    std::wcout << var.ulVal; break;
        case VT_R8:     std::wcout << var.dblVal; break;
        default:        std::wcout << L"(vt=" << var.vt << L")"; break;
        }
        std::wcout << std::endl;
        PropVariantClear(&var);
    }
    else
    {
        std::wcout << L"  " << label << L": (hr=0x" << std::hex << hr << std::dec << L")" << std::endl;
    }
}

int wmain(int argc, wchar_t* argv[])
{
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));

    if (argc < 2) { std::wcout << L"Usage: TestExif.exe <rw2>" << std::endl; return 1; }

    CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    {
        IWICImagingFactory* pFactory = nullptr;
        CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFactory));

        IWICBitmapDecoder* pDecoder = nullptr;
        HRESULT hr = pFactory->CreateDecoderFromFilename(argv[1], nullptr,
            GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
        std::wcout << L"CreateDecoderFromFilename: 0x" << std::hex << hr << std::dec << std::endl;

        if (SUCCEEDED(hr))
        {
            // Frame-level metadata (this is the standard way for WIC)
            IWICBitmapFrameDecode* pFrame = nullptr;
            hr = pDecoder->GetFrame(0, &pFrame);
            std::wcout << L"GetFrame: 0x" << std::hex << hr << std::dec << std::endl;

            if (SUCCEEDED(hr))
            {
                IWICMetadataQueryReader* pReader = nullptr;
                hr = pFrame->GetMetadataQueryReader(&pReader);
                std::wcout << L"GetMetadataQueryReader: 0x" << std::hex << hr << std::dec << std::endl;

                if (SUCCEEDED(hr))
                {
                    std::wcout << std::endl << L"=== EXIF Metadata ===" << std::endl;
                    QueryAndPrint(pReader, L"/{ushort=271}", L"Make");
                    QueryAndPrint(pReader, L"/{ushort=272}", L"Model");
                    QueryAndPrint(pReader, L"/ifd/{ushort=271}", L"Make (ifd)");
                    QueryAndPrint(pReader, L"/ifd/exif/{ushort=33434}", L"ExposureTime");
                    QueryAndPrint(pReader, L"/ifd/exif/{ushort=33437}", L"FNumber");
                    QueryAndPrint(pReader, L"/ifd/exif/{ushort=34855}", L"ISO");
                    QueryAndPrint(pReader, L"/ifd/exif/{ushort=37386}", L"FocalLength");
                    QueryAndPrint(pReader, L"/ifd/exif/{ushort=36867}", L"DateTimeOriginal");
                    QueryAndPrint(pReader, L"/ifd/{ushort=274}", L"Orientation");
                    QueryAndPrint(pReader, L"/ifd/{ushort=256}", L"Width");
                    QueryAndPrint(pReader, L"/ifd/{ushort=257}", L"Height");
                    pReader->Release();
                }
                pFrame->Release();
            }
            pDecoder->Release();
        }
        pFactory->Release();
    }
    CoUninitialize();
    std::wcout << std::endl << L"Done." << std::endl;
    return 0;
}
