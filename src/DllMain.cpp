#include "../include/Common.h"
#include "../include/ClassFactory.h"

LONG g_serverLocks = 0;
LONG g_objectCount = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
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
