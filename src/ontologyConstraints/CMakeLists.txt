add_library(ontologyConstraintsHandler ontologyConstraintsHandler.cpp)

target_include_directories(ontologyConstraintsHandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ontologyConstraintsHandler
    PUBLIC
    causalDiscovery 
    ontology)

install(TARGETS ontologyConstraintsHandler DESTINATION .)