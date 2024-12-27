set(orflib_SOURCES 
    market/market.cpp 
    market/volatilitytermstructure.cpp 
    market/yieldcurve.cpp 
    math/interpol/piecewisepolynomial.cpp 
    math/linalg/choldcmp.cpp 
    math/linalg/eigensym.cpp 
    math/linalg/spectrunc.cpp 
    math/random/sobolurng.cpp 
    math/stats/errorfunction.cpp 
    methods/montecarlo/pathgenerator.cpp 
    methods/pde/pdebase.cpp 
    methods/pde/pde1dsolver.cpp 
    pricers/bsmcpricer.cpp 
    pricers/multiassetbsmcpricer.cpp 
    pricers/ptpricers.cpp     
    pricers/simplepricers.cpp 
)

add_library(orflib STATIC ${orflib_SOURCES})

target_include_directories(orflib PRIVATE 
    .. 
    ${THIRDPARTY_DIRECTORY}/armadillo-${ARMA_VERSION}/include
)

# compiler and linker option adjustments for targets
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(orflib PRIVATE "/permissive-")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    target_compile_options(orflib PRIVATE -fPIC)
endif()
