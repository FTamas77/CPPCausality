find_package(GTest REQUIRED)

add_executable(ontologyTest ontologyTest.cpp)

target_link_libraries(ontologyTest 
	ontology
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

target_include_directories(ontologyTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME ontologyTest COMMAND ontologyTest)

