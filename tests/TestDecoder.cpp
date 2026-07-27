#include <windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <iostream>
#include <string>
#include <wrl/client.h>

#pragma comment(lib, "windowscodecs.lib")

using Microsoft::WRL::ComPtr;

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

    bool success = false;
    {
        ComPtr<IWICImagingFactory> pFactory;
        ComPtr<IWICBitmapDecoder> pDecoder;
        ComPtr<IWICBitmapFrameDecode> pFrame;
        ComPtr<IWICStream> pStream;

        // Create WIC factory
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(pFactory.GetAddressOf())
        );

        if (SUCCEEDED(hr))
        {
            // Create stream
            hr = pFactory->CreateStream(pStream.GetAddressOf());
        }

        if (SUCCEEDED(hr))
        {
            // Initialize stream from file
            hr = pStream->InitializeFromFilename(filePath, GENERIC_READ);
        }

        if (SUCCEEDED(hr))
        {
            // Create decoder from stream
            hr = pFactory->CreateDecoderFromStream(
                pStream.Get(),
                nullptr,
                WICDecodeMetadataCacheOnDemand,
                pDecoder.GetAddressOf()
            );
        }

        if (FAILED(hr))
        {
            PrintError("Failed to initialize decoder", hr);
        }
        else
        {
            std::wcout << L"✓ Successfully created decoder for: " << filePath << std::endl;

            // Get frame count
            UINT frameCount = 0;
            hr = pDecoder->GetFrameCount(&frameCount);
            if (SUCCEEDED(hr))
            {
                std::wcout << L"  Frame count: " << frameCount << std::endl;
            }

            // Get first frame
            hr = pDecoder->GetFrame(0, pFrame.GetAddressOf());
            if (FAILED(hr))
            {
                PrintError("Failed to get frame", hr);
            }
            else
            {
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

                ComPtr<IWICStream> pOutputStream;
                ComPtr<IWICBitmapEncoder> pEncoder;

                    hr = pFactory->CreateStream(pOutputStream.GetAddressOf());
                if (SUCCEEDED(hr))
                {
                    hr = pOutputStream->InitializeFromFilename(outputPath.c_str(), GENERIC_WRITE);
                }

                if (SUCCEEDED(hr))
                {
                    hr = pFactory->CreateEncoder(GUID_ContainerFormatBmp, nullptr, pEncoder.GetAddressOf());
                }

                if (SUCCEEDED(hr))
                {
                    hr = pEncoder->Initialize(pOutputStream.Get(), WICBitmapEncoderNoCache);
                }

                ComPtr<IWICBitmapFrameEncode> pEncodeFrame;
                if (SUCCEEDED(hr))
                {
                    hr = pEncoder->CreateNewFrame(pEncodeFrame.GetAddressOf(), nullptr);
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
                    hr = pEncodeFrame->WriteSource(pFrame.Get(), nullptr);
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
                    success = true;
                }
                else
                {
                    std::wcout << L"✗ Failed to save output image" << std::endl;
                }
            }
        }
    }

    CoUninitialize();
    return success;
}

int wmain(int argc, wchar_t* argv[])
{
    // Set locale to support wide character printing in the terminal
    std::locale::global(std::locale(""));
    std::wcout.imbue(std::locale(""));
    std::wcerr.imbue(std::locale(""));

    if (argc >= 2 && (std::wstring(argv[1]) == L"--help" || std::wstring(argv[1]) == L"-h"))
    {
        std::wcout << L"Usage: TestDecoder.exe <path_to_rw2_file>" << std::endl;
        return 0;
    }

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
