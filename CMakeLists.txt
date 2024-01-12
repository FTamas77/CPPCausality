cmake_minimum_required (VERSION 3.12)

project(cppcausality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_FIND_DEBUG_MODE 1)

# needed for Python
include(GenerateExportHeader)

if(MSVC)
    include(conanbuildinfo.cmake)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    include(/home/umc/CPPCausality/conanbuildinfo.cmake)

    # remote pybind11
    find_package(Python3 REQUIRED)
    set(pybind11_DIR /home/umc/.local/lib/python3.9/site-packages/pybind11)
    find_package(pybind11 REQUIRED HINTS "${pybind11_DIR}")
endif()
    conan_basic_setup()

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
    enable_testing()
endif()

add_subdirectory(src)