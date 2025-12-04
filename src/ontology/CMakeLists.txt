add_library(ontology ontology.cpp classType.cpp property.cpp individual.cpp ontologyXMLReader.cpp)

target_link_libraries(ontology PUBLIC pugixml::pugixml)

target_include_directories(ontology PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ontology DESTINATION .)

if(RUN_TESTS)
    add_subdirectory(tests)
endif()
