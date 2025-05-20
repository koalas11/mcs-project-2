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
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/dct2*.c")
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
    C_EXTENSIONS ON
)

file(GLOB_RECURSE FFTW_FILES "${SRC_DIR}/fftw*.c")
add_executable(dct2_bench "${SRC_FILES}" "${FFTW_FILES}" "${SRC_DIR}/bench-dct2.c")
set_target_properties(dct2_bench PROPERTIES
    OUTPUT_NAME "dct2_bench"
    OUTPUT_NAME_RELEASE "dct2_bench"
    OUTPUT_NAME_DEBUG "dct2_bench"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}"

    C_STANDARD 17
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS ON
)

add_executable(dct2_test "${SRC_FILES}" "${FFTW_FILES}" "${SRC_DIR}/tests-dct2.c")
set_target_properties(dct2_test PROPERTIES
    OUTPUT_NAME "dct2_test"
    OUTPUT_NAME_RELEASE "dct2_test"
    OUTPUT_NAME_DEBUG "dct2_test"
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${BIN_DIR}"

    C_STANDARD 17
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS ON
)

target_include_directories(dct2 PRIVATE ${SRC_DIR})
if(Vulkan_FOUND)
    target_compile_definitions(dct2 PRIVATE HAS_VULKAN)
    target_include_directories(dct2 PRIVATE ${Vulkan_INCLUDE_DIRS})
    target_link_libraries(dct2 Vulkan::Vulkan)
    if (APPLE)
        target_link_libraries(dct2 Vulkan::MoltenVK)
    endif()
endif()

if(MSVC)
    target_compile_options(dct2 PRIVATE /W4)
    target_compile_definitions(dct2 PRIVATE
        DLL_EXPORT
        _USE_MATH_DEFINES
    )
    target_compile_options(dct2_bench PRIVATE /W4)
    target_compile_definitions(dct2_bench PRIVATE
        _USE_MATH_DEFINES
    )
    target_compile_options(dct2_test PRIVATE /W4)
    target_compile_definitions(dct2_test PRIVATE
        _USE_MATH_DEFINES
    )
else()
    target_compile_options(dct2 PRIVATE -Wall -Wextra -pedantic)
    target_compile_definitions(dct2 PRIVATE
        DLL_EXPORT
    )
    target_compile_options(dct2_bench PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(dct2_test PRIVATE -Wall -Wextra -pedantic)
endif()

target_link_libraries(dct2_bench PRIVATE
    ${LIB_DIR}/libfftw3-3.lib
    ${LIB_DIR}/libfftw3f-3.lib
)
target_include_directories(dct2_bench PRIVATE "${LIB_DIR}" "${SRC_DIR}")

target_include_directories(dct2_test PRIVATE )
target_link_libraries(dct2_test PRIVATE
    ${LIB_DIR}/libfftw3-3.lib
    ${LIB_DIR}/libfftw3f-3.lib
)
target_include_directories(dct2_test PRIVATE "${LIB_DIR}" "${SRC_DIR}")