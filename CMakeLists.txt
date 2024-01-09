cmake_minimum_required (VERSION 3.12)

project(cppcausality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_FIND_DEBUG_MODE 1)

# needed for Python
include(GenerateExportHeader)

include(conanbuildinfo.cmake)
conan_basic_setup()

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
    enable_testing()
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(src)