add_library(ontology ontology.cpp classType.cpp property.cpp individual.cpp)

# Because I want to use the library in the tests
target_include_directories(ontology PUBLIC .)

install(TARGETS ontology DESTINATION .)

if(RUN_TESTS)
    add_subdirectory(tests)
endif()
