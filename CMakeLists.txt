cmake_minimum_required(VERSION 3.10)

project(DCT2 LANGUAGES C)

# Compiler options
if (MSVC)
	# warning level 4
	add_compile_options(/W4)
	add_compile_definitions(_USE_MATH_DEFINES)
else()
	# additional warnings
	add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(FORCE_SLOW true)

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
target_include_directories(dct2 PUBLIC ${SRC_DIR})
target_compile_definitions(dct2 PRIVATE
				DLL_EXPORT
)

# Aggiungi l'eseguibile di benchmark
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
target_include_directories(dct2_bench PRIVATE ${SRC_DIR})


# Aggiungi l'eseguibile di test
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
target_include_directories(dct2_test PRIVATE ${SRC_DIR})


find_package(fftw3)
if(fftw3_FOUND)
	message("fftw3 found:")
	message("   -- lib: " ${FFTW3_LIBRARIES})
	message("   -- include dirs: " ${FFTW3_INCLUDE_DIRS})
	message("   -- lib dirs: " ${FFTW3_LIBRARY_DIRS})

	if (NOT FORCE_SLOW)
		add_compile_definitions(HAS_FFTW)
	endif()

	target_include_directories(dct2 SYSTEM PRIVATE ${FFTW3_INCLUDE_DIRS})
	target_include_directories(dct2_bench SYSTEM PRIVATE ${FFTW3_INCLUDE_DIRS})
	target_include_directories(dct2_test SYSTEM PRIVATE ${FFTW3_INCLUDE_DIRS})

	target_link_directories(dct2 PRIVATE ${FFTW3_LIBRARY_DIRS})
	target_link_directories(dct2_bench PRIVATE ${FFTW3_LIBRARY_DIRS})
	target_link_directories(dct2_test PRIVATE ${FFTW3_LIBRARY_DIRS})

	target_link_libraries(dct2 PRIVATE ${FFTW3_LIBRARIES})
	target_link_libraries(dct2_bench PRIVATE ${FFTW3_LIBRARIES})
	target_link_libraries(dct2_test PRIVATE ${FFTW3_LIBRARIES})
endif()

if(APPLE)
	set(VulkanComponents MoltenVK)
endif()
# find_package(Vulkan COMPONENTS ${VulkanComponents})
if(Vulkan_FOUND)
	target_compile_definitions(dct2 PRIVATE HAS_VULKAN)
	target_include_directories(dct2 PRIVATE ${Vulkan_INCLUDE_DIRS})
	target_link_libraries(dct2 PRIVATE Vulkan::Vulkan)
	if (APPLE)
		target_link_libraries(dct2 PRIVATE Vulkan::MoltenVK)
	endif()
endif()

if(WIN32)
	target_link_libraries(dct2_bench PRIVATE
								${LIB_DIR}/libfftw3-3.lib
								${LIB_DIR}/libfftw3f-3.lib
								)

	target_include_directories(dct2_bench SYSTEM PRIVATE "${LIB_DIR}")
	target_include_directories(dct2_test SYSTEM PRIVATE "${LIB_DIR}")

	target_include_directories(dct2_bench PRIVATE "${SRC_DIR}")
	target_include_directories(dct2_test PRIVATE "${SRC_DIR}")

	target_link_libraries(dct2_test PRIVATE
								${LIB_DIR}/libfftw3-3.lib
								${LIB_DIR}/libfftw3f-3.lib
				)
endif()
