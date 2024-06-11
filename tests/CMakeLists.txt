# Find the Google Test package
find_package(GTest REQUIRED)

# Create a test executable
add_executable(causalDiscoveryTest causalDiscoveryTest.cpp)

# TODO: GTest::gtest GTest::gtest_main does not work
target_link_libraries(causalDiscoveryTest
	causalDiscovery
	"D:/conan_data/b/gtestcaa086ac267d4/b/build/lib/Release/gtest.lib"
	"D:/conan_data/b/gtestcaa086ac267d4/b/build/lib/Release/gtest_main.lib")

target_include_directories(causalDiscoveryTest PRIVATE "${gtest_INCLUDE_DIRS_RELEASE}")

# Add a test that runs your test executable
add_test(NAME causalDiscoveryTest COMMAND causalDiscoveryTest)
