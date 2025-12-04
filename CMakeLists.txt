cmake_minimum_required(VERSION 3.16)
project(cppcausality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set the CMP0091 policy to NEW (for MSVC runtime library selection)
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS "Install directory: ${CMAKE_INSTALL_PREFIX}")

# Note: Conan toolchain is included via CMakePresets.json (CMAKE_TOOLCHAIN_FILE)
# No need to manually include it here when using presets

# Find packages via Conan-generated CMake files
find_package(Boost REQUIRED COMPONENTS math serialization)
find_package(Eigen3 REQUIRED)
find_package(pugixml REQUIRED)
find_package(GTest REQUIRED)

# Enable testing before adding subdirectories that contain tests
option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
    enable_testing()
endif()

add_subdirectory(src)

if(RUN_TESTS)
    add_subdirectory(tests)
endif()