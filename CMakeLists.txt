cmake_minimum_required(VERSION 3.30)
project(IKM2)

set(CMAKE_CXX_STANDARD 20)

# Статическая линковка для MinGW
if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

add_executable(IKM2 main.cpp br.cpp br.h)

# Настройки для Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release build - optimizing")
    add_compile_options(/O2)
endif()

