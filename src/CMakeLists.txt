add_library(causalDiscovery 
    causalDiscovery.cpp
    dataLoader.cpp
    graph.cpp
    statistic.cpp)

target_include_directories(causalDiscovery PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(causalDiscovery PRIVATE Boost::math)

# Because I want to use the library in the tests
target_include_directories(causalDiscovery PUBLIC ${CMAKE_SOURCE_DIR}/src)

install(TARGETS causalDiscovery DESTINATION .)
