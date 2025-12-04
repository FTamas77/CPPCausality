find_package(GTest REQUIRED)

add_executable(ontologyUnitTest ontologyTest.cpp ontologyXMLReaderTest.cpp)

target_link_libraries(ontologyUnitTest 
    PRIVATE
    ontology
    GTest::gtest
    GTest::gtest_main)

add_test(NAME ontologyUnitTest COMMAND ontologyUnitTest)

