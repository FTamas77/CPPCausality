find_package(GTest REQUIRED)

add_executable(ontologyUnitTest ontologyTest.cpp ontologyXMLReaderTest.cpp)

target_link_libraries(ontologyUnitTest 
	ontology
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest.lib"
	"D:/conan_data/b/gtest7a7aa469c4366/b/build/lib/Debug/gtest_main.lib")

target_include_directories(ontologyUnitTest PRIVATE "${gtest_INCLUDE_DIRS_DEBUG}")

add_test(NAME ontologyUnitTest COMMAND ontologyUnitTest)

