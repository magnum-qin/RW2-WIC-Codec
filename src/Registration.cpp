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

extern HMODULE g_hModule;

HRESULT RegisterCodec()
{
    HRESULT hr = S_OK;
    HKEY hKey = nullptr;
    HKEY hSubKey = nullptr;
    HKEY hPatternKey = nullptr;
    WCHAR szCLSID[40];
    WCHAR szModulePath[MAX_PATH];

    // Get DLL module path (not the calling process)
    GetModuleFileNameW(g_hModule, szModulePath, MAX_PATH);

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

    // ============================================================
    // Register decoder metadata under our own CLSID key
    // WIC's IWICBitmapDecoderInfo reads Patterns, FriendlyName,
    // FileExtensions, etc. from HERE (not from the Instance key)
    // ============================================================
    hr = SetRegistryKeyValue(hKey, L"FriendlyName", L"Panasonic RW2 Decoder");
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyValue(hKey, L"Author", L"RW2-WIC-Codec");
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyValue(hKey, L"FileExtensions", L".rw2,.RW2");
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyValue(hKey, L"MimeTypes", L"image/x-panasonic-rw2");
    if (FAILED(hr)) goto cleanup;

    {
        WCHAR szContainerGUID[40];
        StringFromGUID2(GUID_ContainerFormatRaw, szContainerGUID, ARRAYSIZE(szContainerGUID));
        hr = SetRegistryKeyValue(hKey, L"ContainerFormat", szContainerGUID);
        if (FAILED(hr)) goto cleanup;
    }

    {
        WCHAR szVendorGUID[40];
        StringFromGUID2(GUID_VendorPanasonic, szVendorGUID, ARRAYSIZE(szVendorGUID));
        hr = SetRegistryKeyValue(hKey, L"Vendor", szVendorGUID);
        if (FAILED(hr)) goto cleanup;
    }

    hr = SetRegistryKeyValue(hKey, L"Version", L"1.0.0.0");
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyDWORD(hKey, L"SupportAnimation", 0);
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyDWORD(hKey, L"SupportLossless", 1);
    if (FAILED(hr)) goto cleanup;

    hr = SetRegistryKeyDWORD(hKey, L"SupportMultiframe", 0);
    if (FAILED(hr)) goto cleanup;

    // Register Patterns under our CLSID key
    hr = CreateRegistryKey(hKey, L"Patterns", &hSubKey);
    if (FAILED(hr)) goto cleanup;

    // Pattern 0: Panasonic RW2 signature "II" + 0x0055
    hr = CreateRegistryKey(hSubKey, L"0", &hPatternKey);
    if (FAILED(hr)) goto cleanup;

    {
        hr = SetRegistryKeyDWORD(hPatternKey, L"Position", 0);
        if (FAILED(hr)) goto cleanup;

        hr = SetRegistryKeyDWORD(hPatternKey, L"Length", 4);
        if (FAILED(hr)) goto cleanup;

        BYTE pattern[] = { 0x49, 0x49, 0x55, 0x00 };
        LONG result = RegSetValueExW(hPatternKey, L"Pattern", 0, REG_BINARY,
            pattern, sizeof(pattern));
        hr = HRESULT_FROM_WIN32(result);
        if (FAILED(hr)) goto cleanup;

        BYTE mask[] = { 0xFF, 0xFF, 0xFF, 0xFF };
        result = RegSetValueExW(hPatternKey, L"Mask", 0, REG_BINARY,
            mask, sizeof(mask));
        hr = HRESULT_FROM_WIN32(result);
        if (FAILED(hr)) goto cleanup;

        hr = SetRegistryKeyDWORD(hPatternKey, L"EndOfStream", 0);
        if (FAILED(hr)) goto cleanup;
    }

    RegCloseKey(hPatternKey);
    hPatternKey = nullptr;
    RegCloseKey(hSubKey);
    hSubKey = nullptr;

    // Register as WIC Bitmap Decoder Instance
    RegCloseKey(hKey);
    hKey = nullptr;

    {
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
        if (FAILED(hr)) goto cleanup;

        // ArbitrationPriority: 0 = highest priority, overrides Microsoft's Raw Image Decoder
        hr = SetRegistryKeyDWORD(hKey, L"ArbitrationPriority", 0);
        if (FAILED(hr)) goto cleanup;

        // ============================================================
        // Register Patterns subkey - CRITICAL for WIC file matching!
        // Without this, WIC cannot associate .rw2 files with our codec.
        // RW2 files use TIFF little-endian format: "II" + 0x55 0x00
        // ============================================================
        hr = CreateRegistryKey(hKey, L"Patterns", &hSubKey);
        if (FAILED(hr)) goto cleanup;

        // Pattern 0: Panasonic RW2 signature "II" + 0x0055 (Panasonic variant)
        hr = CreateRegistryKey(hSubKey, L"0", &hPatternKey);
        if (FAILED(hr)) goto cleanup;

        {
            // Position = 0 (start of file)
            hr = SetRegistryKeyDWORD(hPatternKey, L"Position", 0);
            if (FAILED(hr)) goto cleanup;

            // Length = 4 bytes
            hr = SetRegistryKeyDWORD(hPatternKey, L"Length", 4);
            if (FAILED(hr)) goto cleanup;

            // Pattern: "II" (0x49 0x49) + 0x55 0x00 (Panasonic TIFF variant)
            BYTE pattern[] = { 0x49, 0x49, 0x55, 0x00 };
            LONG result = RegSetValueExW(hPatternKey, L"Pattern", 0, REG_BINARY,
                pattern, sizeof(pattern));
            hr = HRESULT_FROM_WIN32(result);
            if (FAILED(hr)) goto cleanup;

            // Mask: all bytes must match
            BYTE mask[] = { 0xFF, 0xFF, 0xFF, 0xFF };
            result = RegSetValueExW(hPatternKey, L"Mask", 0, REG_BINARY,
                mask, sizeof(mask));
            hr = HRESULT_FROM_WIN32(result);
            if (FAILED(hr)) goto cleanup;
        }

        RegCloseKey(hPatternKey);
        hPatternKey = nullptr;

        // Pattern 1: Standard TIFF little-endian "II" + 0x002A
        // (some RW2 files may use standard TIFF magic)
        hr = CreateRegistryKey(hSubKey, L"1", &hPatternKey);
        if (FAILED(hr)) goto cleanup;

        {
            hr = SetRegistryKeyDWORD(hPatternKey, L"Position", 0);
            if (FAILED(hr)) goto cleanup;

            hr = SetRegistryKeyDWORD(hPatternKey, L"Length", 4);
            if (FAILED(hr)) goto cleanup;

            BYTE pattern[] = { 0x49, 0x49, 0x2A, 0x00 };
            LONG result = RegSetValueExW(hPatternKey, L"Pattern", 0, REG_BINARY,
                pattern, sizeof(pattern));
            hr = HRESULT_FROM_WIN32(result);
            if (FAILED(hr)) goto cleanup;

            BYTE mask[] = { 0xFF, 0xFF, 0xFF, 0xFF };
            result = RegSetValueExW(hPatternKey, L"Mask", 0, REG_BINARY,
                mask, sizeof(mask));
            hr = HRESULT_FROM_WIN32(result);
            if (FAILED(hr)) goto cleanup;
        }

        RegCloseKey(hPatternKey);
        hPatternKey = nullptr;
        RegCloseKey(hSubKey);
        hSubKey = nullptr;
    }

    // Register .rw2 file extension to point to our codec
    {
        HKEY hExtKey = nullptr;
        hr = CreateRegistryKey(HKEY_CLASSES_ROOT, L".rw2", &hExtKey);
        if (SUCCEEDED(hr))
        {
            SetRegistryKeyValue(hExtKey, L"Content Type", L"image/x-panasonic-rw2");
            SetRegistryKeyValue(hExtKey, L"PerceivedType", L"image");
            RegCloseKey(hExtKey);
        }
    }

cleanup:
    if (hPatternKey)
        RegCloseKey(hPatternKey);
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
