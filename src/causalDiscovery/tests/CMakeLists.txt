find_package(GTest REQUIRED)

add_executable(statisticTest statisticTest.cpp)

target_link_libraries(statisticTest
	causalDiscovery 
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

target_include_directories(statisticTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME statisticTest COMMAND statisticTest)

