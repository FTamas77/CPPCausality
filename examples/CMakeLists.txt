cmake_minimum_required(VERSION 3.14)

project(CausalDiscoveryExample VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main_example main.cpp)

# TODO: change this to your own path
target_include_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../src/include)
target_link_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../build/Debug/Debug)

target_link_libraries(main_example PRIVATE causalDiscovery csvreader)

target_compile_definitions(main_example PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
