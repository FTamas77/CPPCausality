function(add_post_build_copy target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Starting to copy KV-41762_202301_test.csv for ${target}"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/tests/KV-41762_202301_test.csv"
            "${CMAKE_BINARY_DIR}/$(Configuration)/KV-41762_202301_test.csv"
        COMMENT "Copying KV-41762_202301_test.csv to the binary directory for ${target}"
    )
endfunction()

add_library(csvreader STATIC CSVReader.cpp)
target_link_libraries(csvreader causalDiscovery)

find_package(GTest REQUIRED)

add_executable(functionalTest causalDiscoveryTest.cpp dataFromOntologyTest.cpp)

# TODO: GTest::gtest GTest::gtest_main does not work
message(STATUS "gtest_LIBRARIES_DEBUG: ${gtest_LIBRARIES_DEBUG}")
target_link_libraries(functionalTest
    causalDiscovery 
    ontology
    csvreader
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

message(STATUS "gtest_INCLUDE_DIRS_DEBUG: ${gtest_INCLUDE_DIRS_DEBUG}")
target_include_directories(functionalTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME functionalTest COMMAND functionalTest)

add_post_build_copy(functionalTest)

add_executable(ontologyWithDiscoveryTest ontologyWithDiscoveryTest.cpp)

target_link_libraries(ontologyWithDiscoveryTest
    causalDiscovery 
    ontology
    ontologyConstraintsHandler
    csvreader
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

target_include_directories(ontologyWithDiscoveryTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME ontologyWithDiscoveryTest COMMAND ontologyWithDiscoveryTest)

add_post_build_copy(ontologyWithDiscoveryTest)