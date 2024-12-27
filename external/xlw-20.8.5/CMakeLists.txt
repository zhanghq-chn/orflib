cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)

# PLATFORM_TARGET is a variable to be defined by the cmake caller
# In Visual Studio IDE this is set in the file CMakeSettings.json

project(xlw)

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
	# use /Z7 for Debug configs
	string(REGEX REPLACE "/Z[iI7]" "" TEMP "${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "${TEMP} /Z7 /wd4244 /wd4996 /wd4838")
    set(CMAKE_CXX_FLAGS_RELEASE "/wd4244 /wd4996 /wd4838")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_DEBUG "-Wno-deprecated -Wno-narrowing")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wno-deprecated -Wno-narrowing")
else()
    message(FATAL_ERROR "unknown compiler; only MSVC and GNU are currenlty supported" )
endif()

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_subdirectory(xlw)
endif()
