cmake_minimum_required(VERSION 3.25)

project(CoroWeaver CXX)

# ── Global settings ─────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, defaulting to Debug")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# ── Output directories (mirrors Axle's layout) ───────────────────────────────
set(OUTPUT_BASE "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_BASE}/$<CONFIG>/${CMAKE_SYSTEM_NAME}-x64")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_BASE}/$<CONFIG>/${CMAKE_SYSTEM_NAME}-x64")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_BASE}/$<CONFIG>/${CMAKE_SYSTEM_NAME}-x64")

# ── Compile commands for clangd ────────────────────────────────────────────────
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Sanitizers ────────────────────────────────────────────────────────────────
option(ENABLE_ASAN "Enable Address Sanitizer" OFF)

if(ENABLE_ASAN)
    if(MSVC)
        add_compile_options(/fsanitize=address)
        add_link_options(/fsanitize=address)
    else()
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
endif()

option(ENABLE_TSAN "Enable Thread Sanitizer" OFF)

if(ENABLE_TSAN)
    if(MSVC)
        message(WARNING "Thread Sanitizer is not supported on MSVC")
    else()
        add_compile_options(-fsanitize=thread)
        add_link_options(-fsanitize=thread)
    endif()
endif()

if(ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "ENABLE_ASAN and ENABLE_TSAN cannot be enabled at the same time")
endif()

# ── Options ───────────────────────────────────────────────────────────────────
option(BUILD_TESTS      "Build the test suite"      ON)
option(BUILD_BENCHMARKS "Build the benchmarks"      ON)

# ── Library ───────────────────────────────────────────────────────────────────
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# ── Amalgamation target ────────────────────────────────────────────────────────
find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_target(amalgamate
    COMMAND ${Python3_EXECUTABLE}
            "${CMAKE_SOURCE_DIR}/scripts/amalgamate.py"
            --root   "${CMAKE_SOURCE_DIR}/src"
            --entry  "CoroWeaver.hpp"
            --output "${CMAKE_SOURCE_DIR}/include/coroweaver/CoroWeaver.hpp"
            --license "${CMAKE_SOURCE_DIR}/LICENSE"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Amalgamating job system into include/coroweaver/CoroWeaver.hpp"
    VERBATIM
)

add_custom_target(amalgamate_debug
    COMMAND ${Python3_EXECUTABLE}
            "${CMAKE_SOURCE_DIR}/scripts/amalgamate.py"
            --root   "${CMAKE_SOURCE_DIR}/src"
            --entry  "CoroWeaver.hpp"
            --output "${CMAKE_SOURCE_DIR}/include/coroweaver/CoroWeaver.hpp"
            --license "${CMAKE_SOURCE_DIR}/LICENSE"
            --remap
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Amalgamating (with #line remapping) into include/coroweaver/CoroWeaver.hpp"
    VERBATIM
)
