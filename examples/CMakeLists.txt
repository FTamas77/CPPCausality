cmake_minimum_required(VERSION 3.16)

project(CausalDiscoveryExample VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main example
add_executable(main_example main.cpp)

if(TARGET causalDiscovery)
    target_link_libraries(main_example PRIVATE causalDiscovery csvreader)
else()
    target_include_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../src/include)
    target_link_directories(main_example PRIVATE ${CMAKE_SOURCE_DIR}/../build)
    target_link_libraries(main_example PRIVATE causalDiscovery csvreader)
endif()

target_compile_definitions(main_example PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Paper reproduction benchmark
add_executable(benchmark_paper benchmark_paper.cpp)

if(TARGET causalDiscovery)
    target_link_libraries(benchmark_paper PRIVATE 
        causalDiscovery 
        csvreader 
        ontology 
        ontologyConstraintsHandler)
else()
    target_include_directories(benchmark_paper PRIVATE ${CMAKE_SOURCE_DIR}/../src/include)
    target_link_directories(benchmark_paper PRIVATE ${CMAKE_SOURCE_DIR}/../build)
    target_link_libraries(benchmark_paper PRIVATE 
        causalDiscovery 
        csvreader 
        ontology 
        ontologyConstraintsHandler)
endif()

target_compile_definitions(benchmark_paper PRIVATE PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

# Copy test CSV to benchmark executable directory
if(EXISTS "${CMAKE_SOURCE_DIR}/../tests/KV-41762_202301_test.csv")
    add_custom_command(TARGET benchmark_paper POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/../tests/KV-41762_202301_test.csv"
            "$<TARGET_FILE_DIR:benchmark_paper>/KV-41762_202301_test.csv"
        COMMENT "Copying test dataset for benchmark_paper"
    )
endif()
