cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 14)

# PLATFORM_TARGET is a variable to be defined by the cmake caller on Windows.
# On macOS/Linux it defaults if not set.
# In Visual Studio IDE this is set in the file CMakeSettings.json
# In VSCode with CMake Tools extension this is set in the file cmake-variants.json
# In command line use, call cmake as follows
#   Windows:  build> cmake -DPLATFORM_TARGET=x64 ..
#   macOS:    build> cmake ..   (defaults to arm64 or x64)
if(NOT DEFINED PLATFORM_TARGET)
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        if(CMAKE_OSX_ARCHITECTURES MATCHES "arm64")
            set(PLATFORM_TARGET "arm64")
        else()
            set(PLATFORM_TARGET "x64")
        endif()
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(PLATFORM_TARGET ${CMAKE_SYSTEM_PROCESSOR})
    else()
        set(PLATFORM_TARGET "x64")
    endif()
endif()

# project-wide configurations
set(THIRDPARTY_DIRECTORY ${CMAKE_SOURCE_DIR}/external)
set(ARMA_VERSION 14.0.2)
set(XLW_VERSION 20.8.5)

project(orflib)

# set location of artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${PLATFORM_TARGET})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${PLATFORM_TARGET})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PLATFORM_TARGET})

# set the debug postfix
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_DEBUG_POSTFIX "-gd")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_DEBUG_POSTFIX "")
endif()

# global compiler and linker option adjustments
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/EHsc /permissive-")
	# use /Zi for Debug configs
	string(REGEX REPLACE "/Z[iI7]" "" TEMP "${CMAKE_CXX_FLAGS_DEBUG}")
    set(CMAKE_CXX_FLAGS_DEBUG "${TEMP} /Zi")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(CMAKE_CXX_FLAGS "-fPIC -Wno-deprecated-declarations -Wno-attributes")
else()
    message(FATAL_ERROR "unknown compiler; only MSVC, GNU, and AppleClang are currently supported" )
endif()

add_subdirectory(orflib)
if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_subdirectory(xlorflib)
endif()
add_subdirectory(pyorflib)
