find_package(GTest REQUIRED)

add_executable(ontologyUnitTest ontologyTest.cpp ontologyXMLReaderTest.cpp)

target_link_libraries(ontologyUnitTest 
	ontology
    ${GTEST_LIB}
    ${GTEST_MAIN_LIB})

target_include_directories(ontologyUnitTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME ontologyUnitTest COMMAND ontologyUnitTest)

