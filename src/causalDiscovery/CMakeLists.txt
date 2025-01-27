add_library(causalDiscovery 
    causalDiscovery.cpp
    causalDiscoveryAPI.cpp
    dataLoader.cpp
    graph.cpp
    statistic.cpp)

find_package(Eigen3 REQUIRED)

# TODO: use variable
set(INCLUDE_DIR ../include)
add_library(causalDiscovery_interface INTERFACE)
target_include_directories(causalDiscovery_interface INTERFACE ${INCLUDE_DIR})

target_include_directories(causalDiscovery PUBLIC ${Boost_INCLUDE_DIRS} .)
target_link_libraries(causalDiscovery PUBLIC causalDiscovery_interface Boost::serialization Boost::math Eigen3::Eigen)

# Because I want to use the library in the tests
#target_include_directories(causalDiscovery PUBLIC .)

install(TARGETS causalDiscovery DESTINATION .)

if(RUN_TESTS)
    add_subdirectory(tests)
endif()