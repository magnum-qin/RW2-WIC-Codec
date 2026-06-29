#include "../include/Common.h"
#include <string>
#include <mutex>

extern HMODULE g_hModule;

HRESULT EnsureDependenciesLoaded()
{
    static std::once_flag flag;
    static HRESULT hrLoad = S_OK;

    std::call_once(flag, []() {
        WCHAR dllPath[MAX_PATH];
        if (GetModuleFileNameW(g_hModule, dllPath, MAX_PATH) == 0)
        {
            hrLoad = HRESULT_FROM_WIN32(GetLastError());
            return;
        }

        WCHAR* lastSlash = wcsrchr(dllPath, L'\\');
        if (lastSlash) *lastSlash = L'\0';

        std::wstring dir = dllPath;
        
        // Load in dependency order: zlib1 -> lcms2-2 -> raw
        HMODULE hZlib = LoadLibraryExW((dir + L"\\zlib1.dll").c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (!hZlib)
        {
            hrLoad = HRESULT_FROM_WIN32(GetLastError());
            return;
        }

        HMODULE hLcms = LoadLibraryExW((dir + L"\\lcms2-2.dll").c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (!hLcms)
        {
            hrLoad = HRESULT_FROM_WIN32(GetLastError());
            return;
        }

        HMODULE hRaw = LoadLibraryExW((dir + L"\\raw.dll").c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (!hRaw)
        {
            hrLoad = HRESULT_FROM_WIN32(GetLastError());
            return;
        }
    });

    return hrLoad;
}
