add_executable(causalDiscovery 
    causalDiscovery.cpp
    dataLoader.cpp
    graph.cpp
    main.cpp
    statistic.cpp)

target_include_directories(causalDiscovery PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(causalDiscovery PRIVATE Boost::math)

install(TARGETS causalDiscovery DESTINATION .)
install(FILES data.csv DESTINATION .)
