#pragma once

#include <windows.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <objbase.h>
#include <shlwapi.h>
#include <memory>
#include <vector>

// Custom GUIDs for RW2 Codec
// Declared here, defined in Guids.cpp via INITGUID

// CLSID for the decoder - {8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}
EXTERN_C const GUID CLSID_RW2Decoder;

// Container format GUID - {FE99CE60-F19C-433C-A3AE-00ACEFA9CA21}
EXTERN_C const GUID GUID_ContainerFormatRaw;

// Vendor GUID (Panasonic) - {8F3E8E61-9C1A-4B3D-A5E1-7C9F8B2D4A3C}
EXTERN_C const GUID GUID_VendorPanasonic;

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
