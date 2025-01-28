find_package(GTest REQUIRED)

add_executable(statisticUnitTest statisticTest.cpp)

target_link_libraries(statisticUnitTest
	causalDiscovery 
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

target_include_directories(statisticUnitTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME statisticUnitTest COMMAND statisticUnitTest)

