cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 11)

# PLATFORM_TARGET is a variable to be defined by the cmake caller
# In Visual Studio IDE this is set in the file CMakeSettings.json
# In VSCode with CMake Tools extension this is set in the file cmake-variants.json
# In command line use, call cmake as follows
# build> cmake -DPLATFORM_TARGET=x64 ..

project(examples)

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
	# use /Zi for Debug configs
	string(REGEX REPLACE "/Z[iI7]" "" TEMP "${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "${TEMP} /Zi")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # do nothing
else()
    message(FATAL_ERROR "unknown compiler; only MSVC and GNU are currenlty supported" )
endif()


# define targets
set(example1_SOURCES example1.cpp)
add_executable(example1 ${example1_SOURCES})
target_include_directories(example1 PRIVATE ../include)

# compiler and linker option adjustments
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS "/EHsc /permissive-")
	# use /Zi for Debug configs
	string(REGEX REPLACE "/Z[iI7]" "" TEMP "${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "${TEMP} /Zi")
    target_link_options(example1 PRIVATE /MANIFEST:NO /INCREMENTAL:NO)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # do nothing
else()
    message(FATAL_ERROR "unknown compiler; only MSVC and GNU are currently supported" )
endif()

# linker option adjustments for supported OS and compilers
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # link libraries and options
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_libraries(example1 PRIVATE
			${CMAKE_SOURCE_DIR}/../lib/${PLATFORM_TARGET}/blas${CMAKE_DEBUG_POSTFIX}.lib 
			${CMAKE_SOURCE_DIR}/../lib/${PLATFORM_TARGET}/lapack${CMAKE_DEBUG_POSTFIX}.lib 
			${CMAKE_SOURCE_DIR}/../lib/${PLATFORM_TARGET}/f2c${CMAKE_DEBUG_POSTFIX}.lib 
		)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_libraries(example1 PRIVATE 
            ${CMAKE_SOURCE_DIR}/../lib/${PLATFORM_TARGET}/libopenblas.a 
            libgfortran.a libquadmath.a
        )
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux") 
    # link libraries and options (assuming GNU)
    target_link_libraries(example1 PRIVATE 
        ### use the three lines below to link against the f2c-ed BLAS/LAPACK packaged with armadillo
        ${CMAKE_SOURCE_DIR}/../lib/${PLATFORM_TARGET}/liblapack${CMAKE_DEBUG_POSTFIX}.a  
        ${CMAKE_SOURCE_DIR}/../lib/${PLATFORM_TARGET}/libblas${CMAKE_DEBUG_POSTFIX}.a  
        ${CMAKE_SOURCE_DIR}/../lib/${PLATFORM_TARGET}/libf2c${CMAKE_DEBUG_POSTFIX}.a  
        ### use the four lines below to link against BLAS/LAPACK pre-installed on the Linux machine
        # /usr/lib/x86_64-linux-gnu/lapack/liblapack.a         
        # /usr/lib/x86_64-linux-gnu/blas/libblas.a 
        # /usr/lib/x86_64-linux-gnu/libgfortran.so.4 
        # /usr/lib/x86_64-linux-gnu/libquadmath.so.0
    )
endif()

# fix the debug executable name
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set_target_properties(example1 PROPERTIES OUTPUT_NAME example1${CMAKE_DEBUG_POSTFIX})
endif()
