#include "../include/Common.h"
#include <strsafe.h>

HRESULT CreateRegistryKey(HKEY hKeyParent, LPCWSTR lpszKeyName, HKEY* phKey)
{
    LONG result = RegCreateKeyExW(
        hKeyParent,
        lpszKeyName,
        0,
        nullptr,
        REG_OPTION_NON_VOLATILE,
        KEY_WRITE,
        nullptr,
        phKey,
        nullptr
    );

    return HRESULT_FROM_WIN32(result);
}

HRESULT SetRegistryKeyValue(HKEY hKey, LPCWSTR lpszValueName, LPCWSTR lpszValue)
{
    DWORD cbData = static_cast<DWORD>((wcslen(lpszValue) + 1) * sizeof(WCHAR));
    LONG result = RegSetValueExW(
        hKey,
        lpszValueName,
        0,
        REG_SZ,
        reinterpret_cast<const BYTE*>(lpszValue),
        cbData
    );

    return HRESULT_FROM_WIN32(result);
}

HRESULT SetRegistryKeyDWORD(HKEY hKey, LPCWSTR lpszValueName, DWORD dwValue)
{
    LONG result = RegSetValueExW(
        hKey,
        lpszValueName,
        0,
        REG_DWORD,
        reinterpret_cast<const BYTE*>(&dwValue),
        sizeof(DWORD)
    );

    return HRESULT_FROM_WIN32(result);
}

HRESULT RegisterCodec()
{
    HRESULT hr = S_OK;
    HKEY hKey = nullptr;
    HKEY hSubKey = nullptr;
    WCHAR szCLSID[40];
    WCHAR szModulePath[MAX_PATH];

    // Get module path
    GetModuleFileNameW(nullptr, szModulePath, MAX_PATH);

    // Convert CLSID to string
    StringFromGUID2(CLSID_RW2Decoder, szCLSID, ARRAYSIZE(szCLSID));

    // Register CLSID
    WCHAR szCLSIDKey[256];
    StringCchPrintfW(szCLSIDKey, ARRAYSIZE(szCLSIDKey), L"CLSID\\%s", szCLSID);

    hr = CreateRegistryKey(HKEY_CLASSES_ROOT, szCLSIDKey, &hKey);
    if (FAILED(hr)) return hr;

    hr = SetRegistryKeyValue(hKey, nullptr, L"Panasonic RW2 Decoder");
    if (FAILED(hr)) goto cleanup;

    // Register InprocServer32
    hr = CreateRegistryKey(hKey, L"InprocServer32", &hSubKey);
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyValue(hSubKey, nullptr, szModulePath);
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyValue(hSubKey, L"ThreadingModel", L"Both");
    if (FAILED(hr)) goto cleanup;

    RegCloseKey(hSubKey);
    hSubKey = nullptr;

    // Register as WIC Bitmap Decoder
    RegCloseKey(hKey);
    hKey = nullptr;

    WCHAR szDecoderKey[512];
    StringCchPrintfW(szDecoderKey, ARRAYSIZE(szDecoderKey),
        L"CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance\\%s", szCLSID);

    hr = CreateRegistryKey(HKEY_CLASSES_ROOT, szDecoderKey, &hKey);
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyValue(hKey, L"CLSID", szCLSID);
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyValue(hKey, L"FriendlyName", L"Panasonic RW2 Decoder");
    if (FAILED(hr)) goto cleanup;

    // Register file extensions
    hr = SetRegistryKeyValue(hKey, L"FileExtensions", L".rw2,.RW2");
    if (FAILED(hr)) goto cleanup;

    // Register MIME types
    hr = SetRegistryKeyValue(hKey, L"MimeTypes", L"image/x-panasonic-rw2");
    if (FAILED(hr)) goto cleanup;

    // Vendor GUID
    WCHAR szVendorGUID[40];
    StringFromGUID2(GUID_VendorPanasonic, szVendorGUID, ARRAYSIZE(szVendorGUID));
    hr = SetRegistryKeyValue(hKey, L"Vendor", szVendorGUID);
    if (FAILED(hr)) goto cleanup;

    // Version
    hr = SetRegistryKeyValue(hKey, L"Version", L"1.0.0.0");
    if (FAILED(hr)) goto cleanup;

    // Supports lossless
    hr = SetRegistryKeyDWORD(hKey, L"SupportsLossless", 1);
    if (FAILED(hr)) goto cleanup;

    // Container format
    WCHAR szContainerGUID[40];
    StringFromGUID2(GUID_ContainerFormatRaw, szContainerGUID, ARRAYSIZE(szContainerGUID));
    hr = SetRegistryKeyValue(hKey, L"ContainerFormat", szContainerGUID);

cleanup:
    if (hSubKey)
        RegCloseKey(hSubKey);
    if (hKey)
        RegCloseKey(hKey);

    return hr;
}

HRESULT UnregisterCodec()
{
    WCHAR szCLSID[40];
    StringFromGUID2(CLSID_RW2Decoder, szCLSID, ARRAYSIZE(szCLSID));

    // Delete CLSID key
    WCHAR szCLSIDKey[256];
    StringCchPrintfW(szCLSIDKey, ARRAYSIZE(szCLSIDKey), L"CLSID\\%s", szCLSID);
    RegDeleteTreeW(HKEY_CLASSES_ROOT, szCLSIDKey);

    // Delete instance key
    WCHAR szDecoderKey[512];
    StringCchPrintfW(szDecoderKey, ARRAYSIZE(szDecoderKey),
        L"CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance\\%s", szCLSID);
    RegDeleteTreeW(HKEY_CLASSES_ROOT, szDecoderKey);

    return S_OK;
}
