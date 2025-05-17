cmake_minimum_required(VERSION 3.10)

project(DCT2 LANGUAGES C)

if(APPLE)
    set(VulkanComponents MoltenVK)
endif()

find_package(Vulkan COMPONENTS ${VulkanComponents})

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

    LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}"

    C_STANDARD 17
    C_STANDARD_REQUIRED YES
)

target_include_directories(dct2 PRIVATE ${SRC_DIR})
if(Vulkan_FOUND)
    target_compile_definitions(dct2 PRIVATE HAS_VULKAN)
    target_link_libraries(dct2 Vulkan::Vulkan Vulkan::MoltenVK)
endif()

if(MSVC)
    target_compile_options(dct2 PRIVATE /W4)
    target_compile_definitions(dct2 PRIVATE
        DLL_EXPORT
        _USE_MATH_DEFINES
    )
else()
    target_compile_options(dct2 PRIVATE -Wall -Wextra -pedantic)
    target_compile_definitions(dct2 PRIVATE
        DLL_EXPORT
    )
endif()
