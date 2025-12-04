function(add_post_build_copy target)
    add_custom_command(
        TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Starting to copy KV-41762_202301_test.csv for ${target}"
        COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_SOURCE_DIR}/tests/KV-41762_202301_test.csv"
            "$<TARGET_FILE_DIR:${target}>/KV-41762_202301_test.csv"
        COMMENT "Copying KV-41762_202301_test.csv to the binary directory for ${target}"
    )
endfunction()

add_executable(functionalIntegrationTest causalDiscoveryTest.cpp dataFromOntologyTest.cpp)

target_link_libraries(functionalIntegrationTest
    PRIVATE
    causalDiscovery 
    ontology
    csvreader
    GTest::gtest
    GTest::gtest_main)

add_test(NAME functionalIntegrationTest COMMAND functionalIntegrationTest)
set_tests_properties(functionalIntegrationTest PROPERTIES WORKING_DIRECTORY "$<TARGET_FILE_DIR:functionalIntegrationTest>")
add_post_build_copy(functionalIntegrationTest)

add_executable(ontologyWithDiscoveryIntegrationTest ontologyWithDiscoveryTest.cpp)

target_link_libraries(ontologyWithDiscoveryIntegrationTest
    PRIVATE
    causalDiscovery 
    ontology
    ontologyConstraintsHandler
    csvreader
    GTest::gtest
    GTest::gtest_main)

add_test(NAME ontologyWithDiscoveryIntegrationTest COMMAND ontologyWithDiscoveryIntegrationTest)
set_tests_properties(ontologyWithDiscoveryIntegrationTest PROPERTIES WORKING_DIRECTORY "$<TARGET_FILE_DIR:ontologyWithDiscoveryIntegrationTest>")
add_post_build_copy(ontologyWithDiscoveryIntegrationTest)