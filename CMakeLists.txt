cmake_minimum_required(VERSION 3.10)

project(DCT2 LANGUAGES C)

# Define directories
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(BIN_DIR "${CMAKE_SOURCE_DIR}/target")

# Create output directories (these will be created during the build)
file(MAKE_DIRECTORY ${BIN_DIR})

# Aggiungi la libreria per la DCT-2
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.c")
add_library(dct2 STATIC ${SRC_FILES})
set_target_properties(dct2 PROPERTIES
    OUTPUT_NAME "dct2"
    OUTPUT_NAME_RELEASE "dct2"
    OUTPUT_NAME_DEBUG "dct2"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}"
)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

target_compile_features(dct2 PRIVATE
    c::23
)
