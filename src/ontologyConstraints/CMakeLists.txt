add_library(ontologyConstraintsHandler ontologyConstraintsHandler.cpp)

# Because I want to use the library in the tests
target_include_directories(ontologyConstraintsHandler PUBLIC .)

target_link_libraries(ontologyConstraintsHandler
    causalDiscovery 
    ontology)

install(TARGETS ontologyConstraintsHandler DESTINATION .)