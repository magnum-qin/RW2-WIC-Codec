cmake_minimum_required(VERSION 3.15)
project(RW2Codec VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LibRaw
find_package(LibRaw CONFIG)
if(NOT LibRaw_FOUND)
    message(STATUS "LibRaw not found via CMake, trying vcpkg or manual path")
    find_path(LIBRAW_INCLUDE_DIR libraw/libraw.h)
    find_library(LIBRAW_LIBRARY NAMES raw libraw)
    if(LIBRAW_INCLUDE_DIR AND LIBRAW_LIBRARY)
        add_library(LibRaw::LibRaw UNKNOWN IMPORTED)
        set_target_properties(LibRaw::LibRaw PROPERTIES
            IMPORTED_LOCATION "${LIBRAW_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${LIBRAW_INCLUDE_DIR}"
        )
    else()
        message(FATAL_ERROR "LibRaw not found. Install via vcpkg: vcpkg install libraw:x64-windows")
    endif()
endif()

# Main codec DLL
add_library(RW2Codec SHARED
    src/DllMain.cpp
    src/ClassFactory.cpp
    src/RW2Decoder.cpp
    src/RW2FrameDecode.cpp
    src/Registration.cpp
    src/Guids.cpp
    src/Utils.cpp
    RW2Codec.def
)

target_include_directories(RW2Codec PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(RW2Codec PRIVATE
    LibRaw::LibRaw
    windowscodecs
    uuid
    ole32
    oleaut32
)

# Test program
add_executable(TestDecoder
    tests/TestDecoder.cpp
)

target_link_libraries(TestDecoder PRIVATE
    windowscodecs
    uuid
    ole32
    oleaut32
)

# Installation
install(TARGETS RW2Codec
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(FILES
    scripts/install.bat
    scripts/uninstall.bat
    DESTINATION bin
)
