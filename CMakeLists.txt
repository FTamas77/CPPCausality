cmake_minimum_required (VERSION 3.12)
project(cppcausality LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_FIND_DEBUG_MODE 1)

# Run Conan install if necessary
if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.lock" OR NOT EXISTS "${CMAKE_BINARY_DIR}/conan_toolchain.cmake")
    execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --build=missing --output-folder ${CMAKE_BINARY_DIR})
endif()

# Set the CMP0091 policy to NEW
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()


# Include the Conan toolchain file
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

# Find the packages specified in your conanfile.txt
find_package(boost REQUIRED)

option(RUN_TESTS "Build the tests" ON)
if(RUN_TESTS)
    enable_testing()
endif()

# Link against the Boost libraries
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

add_subdirectory(src)
