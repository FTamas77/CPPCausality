add_library(causalDiscovery 
    causalDiscovery.cpp
    dataLoader.cpp
    graph.cpp
    statistic.cpp)

find_package(Eigen3 REQUIRED)

target_include_directories(causalDiscovery PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(causalDiscovery PRIVATE Boost::serialization Boost::math Eigen3::Eigen)

# Because I want to use the library in the tests
target_include_directories(causalDiscovery PUBLIC ${CMAKE_SOURCE_DIR}/src)

install(TARGETS causalDiscovery DESTINATION .)
