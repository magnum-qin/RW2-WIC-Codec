cmake_minimum_required(VERSION 3.15)
project(RW2Codec VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find LibRaw via vcpkg (package name is "libraw", targets are libraw::raw / libraw::raw_r)
find_package(libraw CONFIG REQUIRED)

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
    libraw::raw
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

add_executable(TestPerf tests/TestPerf.cpp)
target_link_libraries(TestPerf PRIVATE libraw::raw)

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
