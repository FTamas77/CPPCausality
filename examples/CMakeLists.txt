cmake_minimum_required(VERSION 3.16)

project(CausalDiscoveryExample VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(main_example main.cpp)

# Link against the main project libraries
# When building as part of the main project, the targets are available
if(TARGET causalDiscovery)
    target_link_libraries(main_example PRIVATE causalDiscovery csvreader)
else()
    # Standalone build - use paths relative to this directory
    target_include_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../src/include)
    target_link_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../build)
    target_link_libraries(main_example PRIVATE causalDiscovery csvreader)
endif()

target_compile_definitions(main_example PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
