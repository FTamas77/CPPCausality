find_package(GTest REQUIRED)

add_executable(statisticUnitTest statisticTest.cpp)

target_link_libraries(statisticUnitTest
    PRIVATE
    causalDiscovery 
    GTest::gtest
    GTest::gtest_main)

add_test(NAME statisticUnitTest COMMAND statisticUnitTest)

