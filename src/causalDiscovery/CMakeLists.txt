add_library(causalDiscovery 
    causalDiscovery.cpp
    causalDiscoveryAPI.cpp
    graph.cpp
    statistic.cpp)

find_package(Eigen3 REQUIRED)

set(INCLUDE_DIR ../include)

add_library(causalDiscovery_interface INTERFACE)
target_include_directories(causalDiscovery_interface INTERFACE ${INCLUDE_DIR})

# TODO: move into utils
add_library(csvreader STATIC CSVReader.cpp)
target_link_libraries(csvreader causalDiscovery)
install(TARGETS csvreader DESTINATION .)

target_link_libraries(causalDiscovery PUBLIC csvreader causalDiscovery_interface Boost::serialization Boost::math Eigen3::Eigen)

target_include_directories(causalDiscovery PUBLIC ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS causalDiscovery DESTINATION .)

if(RUN_TESTS)
    add_subdirectory(tests)
endif()
