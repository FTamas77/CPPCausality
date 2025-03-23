find_package(GTest REQUIRED)

add_executable(statisticUnitTest statisticTest.cpp)

target_link_libraries(statisticUnitTest
	causalDiscovery 
    ${GTEST_LIB}
    ${GTEST_MAIN_LIB})

target_include_directories(statisticUnitTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME statisticUnitTest COMMAND statisticUnitTest)

