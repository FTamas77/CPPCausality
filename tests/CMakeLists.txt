# Find the Google Test package
find_package(GTest REQUIRED)

# Create a test executable
add_executable(causalDiscoveryTest causalDiscoveryTest.cpp)

# TODO: GTest::gtest GTest::gtest_main does not work
message(STATUS "gtest_LIBRARIES_DEBUG: ${gtest_LIBRARIES_DEBUG}")
target_link_libraries(causalDiscoveryTest
	causalDiscovery 
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

message(STATUS "gtest_INCLUDE_DIRS_DEBUG: ${gtest_INCLUDE_DIRS_DEBUG}")
target_include_directories(causalDiscoveryTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

# Add a test that runs your test executable
add_test(NAME causalDiscoveryTest COMMAND causalDiscoveryTest)

add_custom_command(
    TARGET causalDiscoveryTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/tests/data.csv
            ${CMAKE_BINARY_DIR})

#install(FILES data.csv DESTINATION .)
