#pragma once

#include <windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <objbase.h>
#include <shlwapi.h>
#include <memory>
#include <vector>

// Custom GUIDs for RW2 Codec
// CLSID for the decoder - {8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}
DEFINE_GUID(CLSID_RW2Decoder,
    0x8F3E8E60, 0x9C1A, 0x4B3D,
    0xA5, 0xE1, 0x7C, 0x9F, 0x8B, 0x2D, 0x4A, 0x3C);

// Container format GUID - {FE99CE60-F19C-433C-A3AE-00ACEFA9CA21}
DEFINE_GUID(GUID_ContainerFormatRaw,
    0xFE99CE60, 0xF19C, 0x433C,
    0xA3, 0xAE, 0x00, 0xAC, 0xEF, 0xA9, 0xCA, 0x21);

// Vendor GUID (Panasonic) - {8F3E8E61-9C1A-4B3D-A5E1-7C9F8B2D4A3C}
DEFINE_GUID(GUID_VendorPanasonic,
    0x8F3E8E61, 0x9C1A, 0x4B3D,
    0xA5, 0xE1, 0x7C, 0x9F, 0x8B, 0x2D, 0x4A, 0x3C);

// Helper macros
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = nullptr; } }

// Forward declarations
class ClassFactory;
class RW2Decoder;
class RW2FrameDecode;

// Utility functions
HRESULT RegisterCodec();
HRESULT UnregisterCodec();
HRESULT CreateRegistryKey(HKEY hKeyParent, LPCWSTR lpszKeyName, HKEY* phKey);
HRESULT SetRegistryKeyValue(HKEY hKey, LPCWSTR lpszValueName, LPCWSTR lpszValue);
