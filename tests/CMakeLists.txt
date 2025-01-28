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

find_package(GTest REQUIRED)

add_executable(functionalIntegrationTest causalDiscoveryTest.cpp dataFromOntologyTest.cpp)

# TODO: GTest::gtest GTest::gtest_main does not work
message(STATUS "gtest_LIBRARIES_DEBUG: ${gtest_LIBRARIES_DEBUG}")
target_link_libraries(functionalIntegrationTest
    causalDiscovery 
    ontology
    csvreader
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

message(STATUS "gtest_INCLUDE_DIRS_DEBUG: ${gtest_INCLUDE_DIRS_DEBUG}")
target_include_directories(functionalIntegrationTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME functionalIntegrationTest COMMAND functionalIntegrationTest)

add_post_build_copy(functionalIntegrationTest)

add_executable(ontologyWithDiscoveryIntegrationTest ontologyWithDiscoveryTest.cpp)

target_link_libraries(ontologyWithDiscoveryIntegrationTest
    causalDiscovery 
    ontology
    ontologyConstraintsHandler
    csvreader
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

target_include_directories(ontologyWithDiscoveryIntegrationTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME ontologyWithDiscoveryIntegrationTest COMMAND ontologyWithDiscoveryIntegrationTest)

add_post_build_copy(ontologyWithDiscoveryIntegrationTest)