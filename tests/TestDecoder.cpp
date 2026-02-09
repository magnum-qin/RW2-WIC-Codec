#include <windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <iostream>
#include <string>
#include <atlbase.h>

#pragma comment(lib, "windowscodecs.lib")

void PrintError(const char* message, HRESULT hr)
{
    std::wcerr << message << L" Error: 0x" << std::hex << hr << std::endl;
}

bool TestDecoder(const wchar_t* filePath)
{
    HRESULT hr = S_OK;

    // Initialize COM
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        PrintError("Failed to initialize COM", hr);
        return false;
    }

    CComPtr<IWICImagingFactory> pFactory;
    CComPtr<IWICBitmapDecoder> pDecoder;
    CComPtr<IWICBitmapFrameDecode> pFrame;
    CComPtr<IWICStream> pStream;

    // Create WIC factory
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pFactory)
    );

    if (FAILED(hr))
    {
        PrintError("Failed to create WIC factory", hr);
        CoUninitialize();
        return false;
    }

    // Create stream
    hr = pFactory->CreateStream(&pStream);
    if (FAILED(hr))
    {
        PrintError("Failed to create stream", hr);
        CoUninitialize();
        return false;
    }

    // Initialize stream from file
    hr = pStream->InitializeFromFilename(filePath, GENERIC_READ);
    if (FAILED(hr))
    {
        PrintError("Failed to open file", hr);
        CoUninitialize();
        return false;
    }

    // Create decoder from stream
    hr = pFactory->CreateDecoderFromStream(
        pStream,
        nullptr,
        WICDecodeMetadataCacheOnDemand,
        &pDecoder
    );

    if (FAILED(hr))
    {
        PrintError("Failed to create decoder", hr);
        CoUninitialize();
        return false;
    }

    std::wcout << L"✓ Successfully created decoder for: " << filePath << std::endl;

    // Get frame count
    UINT frameCount = 0;
    hr = pDecoder->GetFrameCount(&frameCount);
    if (SUCCEEDED(hr))
    {
        std::wcout << L"  Frame count: " << frameCount << std::endl;
    }

    // Get first frame
    hr = pDecoder->GetFrame(0, &pFrame);
    if (FAILED(hr))
    {
        PrintError("Failed to get frame", hr);
        CoUninitialize();
        return false;
    }

    // Get image size
    UINT width = 0, height = 0;
    hr = pFrame->GetSize(&width, &height);
    if (SUCCEEDED(hr))
    {
        std::wcout << L"  Image size: " << width << L" x " << height << std::endl;
    }

    // Get pixel format
    WICPixelFormatGUID pixelFormat;
    hr = pFrame->GetPixelFormat(&pixelFormat);
    if (SUCCEEDED(hr))
    {
        std::wcout << L"  Pixel format retrieved successfully" << std::endl;
    }

    // Get resolution
    double dpiX = 0, dpiY = 0;
    hr = pFrame->GetResolution(&dpiX, &dpiY);
    if (SUCCEEDED(hr))
    {
        std::wcout << L"  Resolution: " << dpiX << L" x " << dpiY << L" DPI" << std::endl;
    }

    // Try to save as BMP
    std::wstring outputPath = filePath;
    size_t dotPos = outputPath.find_last_of(L'.');
    if (dotPos != std::wstring::npos)
    {
        outputPath = outputPath.substr(0, dotPos);
    }
    outputPath += L"_output.bmp";

    CComPtr<IWICStream> pOutputStream;
    CComPtr<IWICBitmapEncoder> pEncoder;

    hr = pFactory->CreateStream(&pOutputStream);
    if (SUCCEEDED(hr))
    {
        hr = pOutputStream->InitializeFromFilename(outputPath.c_str(), GENERIC_WRITE);
    }

    if (SUCCEEDED(hr))
    {
        hr = pFactory->CreateEncoder(GUID_ContainerFormatBmp, nullptr, &pEncoder);
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncoder->Initialize(pOutputStream, WICBitmapEncoderNoCache);
    }

    CComPtr<IWICBitmapFrameEncode> pEncodeFrame;
    if (SUCCEEDED(hr))
    {
        hr = pEncoder->CreateNewFrame(&pEncodeFrame, nullptr);
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncodeFrame->Initialize(nullptr);
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncodeFrame->SetSize(width, height);
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncodeFrame->WriteSource(pFrame, nullptr);
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncodeFrame->Commit();
    }

    if (SUCCEEDED(hr))
    {
        hr = pEncoder->Commit();
    }

    if (SUCCEEDED(hr))
    {
        std::wcout << L"✓ Successfully saved output to: " << outputPath << std::endl;
    }
    else
    {
        std::wcout << L"✗ Failed to save output image" << std::endl;
    }

    CoUninitialize();
    return true;
}

int wmain(int argc, wchar_t* argv[])
{
    std::wcout << L"RW2 Codec Test Program\n" << std::endl;
    std::wcout << L"======================\n" << std::endl;

    if (argc < 2)
    {
        std::wcout << L"Usage: TestDecoder.exe <path_to_rw2_file>" << std::endl;
        std::wcout << L"\nExample: TestDecoder.exe C:\\Photos\\sample.rw2" << std::endl;
        return 1;
    }

    const wchar_t* filePath = argv[1];

    std::wcout << L"Testing RW2 file: " << filePath << L"\n" << std::endl;

    if (TestDecoder(filePath))
    {
        std::wcout << L"\n✓ All tests passed!" << std::endl;
        return 0;
    }
    else
    {
        std::wcout << L"\n✗ Tests failed!" << std::endl;
        return 1;
    }
}
