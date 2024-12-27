set(xlw_SOURCES 
    src/ArgList.cpp 
    src/DoubleOrNothing.cpp 
    src/HiResTimer.cpp 
    src/MJCellMatrix.cpp 
    src/NCmatrices.cpp 
    src/PascalStringConversions.cpp 
    src/PathUpdater.cpp 
    src/TempMemory.cpp 
    src/Win32StreamBuf.cpp 
    src/xlcall.cpp 
    src/XlfAbstractCmdDesc.cpp 
    src/XlfArgDesc.cpp 
    src/XlfArgDescList.cpp 
    src/XlfCmdDesc.cpp 
    src/XlfExcel.cpp 
    src/XlfFuncDesc.cpp 
    src/XlfOperImpl.cpp 
    src/XlfOperProperties.cpp 
    src/XlfRef.cpp 
    src/XlfServices.cpp 
    src/XlFunctionRegistration.cpp 
    src/XlOpenClose.cpp 
)

add_library(xlw STATIC ${xlw_SOURCES})

target_include_directories(xlw PRIVATE include)

# compiler and linker option adjustments for targets
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(xlw PRIVATE /permissive-)
    target_link_options(xlw PRIVATE /MANIFEST:NO /INCREMENTAL:NO)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(xlw PRIVATE -static-libgcc)
endif()
