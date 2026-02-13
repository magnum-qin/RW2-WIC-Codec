#include <initguid.h>
#include <windows.h>
#include <wincodec.h>

// CLSID for the decoder - {8F3E8E60-9C1A-4B3D-A5E1-7C9F8B2D4A3C}
DEFINE_GUID(CLSID_RW2Decoder,
    0x8F3E8E60, 0x9C1A, 0x4B3D,
    0xA5, 0xE1, 0x7C, 0x9F, 0x8B, 0x2D, 0x4A, 0x3C);

// Note: GUID_ContainerFormatRaw is already defined in wincodec.h, no need to redefine

// Vendor GUID (Panasonic) - {8F3E8E61-9C1A-4B3D-A5E1-7C9F8B2D4A3C}
DEFINE_GUID(GUID_VendorPanasonic,
    0x8F3E8E61, 0x9C1A, 0x4B3D,
    0xA5, 0xE1, 0x7C, 0x9F, 0x8B, 0x2D, 0x4A, 0x3C);
