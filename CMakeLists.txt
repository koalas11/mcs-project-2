cmake_minimum_required(VERSION 3.10)

project(DCT2 LANGUAGES C)

find_package(GSL REQUIRED)

# Define directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/target")

# Create output directories (these will be created during the build)
file(MAKE_DIRECTORY ${BIN_DIR})

# Aggiungi la libreria per la DCT-2
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c")
add_library(dct2 SHARED ${SRC_FILES})
set_target_properties(dct2 PROPERTIES
    OUTPUT_NAME "dct2"
    OUTPUT_NAME_RELEASE "dct2"
    OUTPUT_NAME_DEBUG "dct2"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}"

    C_STANDARD 17
    C_STANDARD_REQUIRED YES
)

target_include_directories(dct2 PRIVATE ${SRC_DIR})
target_link_libraries(dct2 GSL::gsl GSL::gslcblas)

if(MSVC)
    target_compile_options(dct2 PRIVATE /W4 /WX)
else()
    target_compile_options(dct2 PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
