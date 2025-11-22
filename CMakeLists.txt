# Enable CMake integration with vcpkg manifest mode
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

cmake_minimum_required(VERSION 4.0)
project(evo)

set(CMAKE_CXX_STANDARD 20)

add_executable(evo main.cpp
        utils/sha1.cpp
        utils/sha1.h
        utils/Platform.h
)
find_package(ZLIB REQUIRED)
target_link_libraries(evo PRIVATE ZLIB::ZLIB)
