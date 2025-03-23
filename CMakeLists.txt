cmake_minimum_required (VERSION 3.12)
project(cppcausality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
#set(CMAKE_FIND_DEBUG_MODE 1)

# Set the output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

# Run Conan install if necessary
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.lock" OR NOT EXISTS "${CMAKE_BINARY_DIR}/build/generators/conan_toolchain.cmake")
    # TODO: I think this is not needed: --output-folder ${CMAKE_BINARY_DIR}
    # it is needed, because the build files goes there
    execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --build=missing --output-folder ${CMAKE_BINARY_DIR})
endif()

# Set the CMP0091 policy to NEW
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

# Debug
message("Install directory: ${CMAKE_INSTALL_PREFIX}")

# Include the Conan toolchain file
include(${CMAKE_BINARY_DIR}/build/generators/conan_toolchain.cmake)

find_package(Boost COMPONENTS math)

# Temporary fix for some conan packages
set(GTEST_INC_DIR "c:/conan_data/b/gtest9909e8a124da0/p/include/")
set(GTEST_LIB_DIR "c:/conan_data/b/gtest9909e8a124da0/p/lib/")
set(GTEST_LIB "${GTEST_LIB_DIR}/gtest.lib")
set(GTEST_MAIN_LIB "${GTEST_LIB_DIR}/gtest_main.lib")

set(EIGEN3_INCLUDE_DIR "C:/conan_data/eigen3d88c0279cc26/p/include/eigen3")

add_subdirectory(src)

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()