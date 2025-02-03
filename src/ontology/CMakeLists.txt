add_library(ontology ontology.cpp classType.cpp property.cpp individual.cpp ontologyXMLReader.cpp)

find_package(pugixml REQUIRED)
target_link_libraries(ontology pugixml::pugixml)

# Because I want to use the library in the tests
target_include_directories(ontology PUBLIC .)

install(TARGETS ontology DESTINATION .)

if(RUN_TESTS)
    add_subdirectory(tests)
endif()
