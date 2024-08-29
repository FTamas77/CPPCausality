# Find the Google Test package
find_package(GTest REQUIRED)

# Create a test executable
add_executable(functionalTest causalDiscoveryTest.cpp dataFromOntologyTest.cpp) # ontologyWithDiscoveryTest.cpp)

# TODO: GTest::gtest GTest::gtest_main does not work
message(STATUS "gtest_LIBRARIES_DEBUG: ${gtest_LIBRARIES_DEBUG}")
target_link_libraries(functionalTest
    causalDiscovery 
    ontology
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
    "D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

message(STATUS "gtest_INCLUDE_DIRS_DEBUG: ${gtest_INCLUDE_DIRS_DEBUG}")
target_include_directories(functionalTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

# Add a test that runs your test executable
add_test(NAME functionalTest COMMAND functionalTest)


message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

add_custom_command(
    TARGET functionalTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Starting to copy KV-41762_202301_test.csv"
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/tests/KV-41762_202301_test.csv"
        "${CMAKE_BINARY_DIR}/$(Configuration)/KV-41762_202301_test.csv"
    COMMENT "Copying KV-41762_202301_test.csv to the binary directory"
)
