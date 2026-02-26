#include "../include/Common.h"
#include "../include/ClassFactory.h"

LONG g_serverLocks = 0;
LONG g_objectCount = 0;
HMODULE g_hModule = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);

        // Add our DLL's directory to the DLL search path so that
        // dependent DLLs (raw.dll, lcms2-2.dll, zlib1.dll) can be found
        // when WIC loads RW2Codec.dll from another process (e.g. explorer.exe)
        {
            WCHAR dllPath[MAX_PATH];
            if (GetModuleFileNameW(hModule, dllPath, MAX_PATH) > 0)
            {
                // Remove filename, keep directory
                WCHAR* lastSlash = wcsrchr(dllPath, L'\\');
                if (lastSlash) *lastSlash = L'\0';
                SetDllDirectoryW(dllPath);
            }
        }
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppv)
{
    if (ppv == nullptr)
        return E_INVALIDARG;

    *ppv = nullptr;

    if (rclsid != CLSID_RW2Decoder)
        return CLASS_E_CLASSNOTAVAILABLE;

    ClassFactory* pFactory = new (std::nothrow) ClassFactory();
    if (pFactory == nullptr)
        return E_OUTOFMEMORY;

    HRESULT hr = pFactory->QueryInterface(riid, ppv);
    pFactory->Release();

    return hr;
}

STDAPI DllCanUnloadNow()
{
    return (g_serverLocks == 0 && g_objectCount == 0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer()
{
    return RegisterCodec();
}

STDAPI DllUnregisterServer()
{
    return UnregisterCodec();
}
