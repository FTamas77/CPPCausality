find_package(GTest REQUIRED)

add_executable(statisticUnitTest statisticTest.cpp)

target_link_libraries(statisticUnitTest
    PRIVATE
    causalDiscovery 
    GTest::gtest
    GTest::gtest_main)

add_test(NAME statisticUnitTest COMMAND statisticUnitTest)

# Graph constraints unit test
add_executable(graphConstraintsUnitTest graphConstraintsTest.cpp)

target_link_libraries(graphConstraintsUnitTest
    PRIVATE
    causalDiscovery 
    GTest::gtest
    GTest::gtest_main)

add_test(NAME graphConstraintsUnitTest COMMAND graphConstraintsUnitTest)

# Causal discovery with constraints unit test
add_executable(causalDiscoveryConstraintsTest causalDiscoveryTest.cpp)

target_link_libraries(causalDiscoveryConstraintsTest
    PRIVATE
    causalDiscovery 
    csvreader
    GTest::gtest
    GTest::gtest_main)

# Copy test data file from tests directory
add_custom_command(TARGET causalDiscoveryConstraintsTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/tests/KV-41762_202301_test.csv"
        "$<TARGET_FILE_DIR:causalDiscoveryConstraintsTest>/KV-41762_202301_test.csv"
    COMMENT "Copying KV-41762_202301_test.csv for causalDiscoveryConstraintsTest"
)

add_test(NAME causalDiscoveryConstraintsTest COMMAND causalDiscoveryConstraintsTest
    WORKING_DIRECTORY $<TARGET_FILE_DIR:causalDiscoveryConstraintsTest>
)

