cmake_minimum_required (VERSION 3.12)

project(cppcausality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_FIND_DEBUG_MODE 1)

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
    enable_testing()
endif()

add_subdirectory(src)