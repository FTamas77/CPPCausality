add_executable(causalDiscovery 
    causalDiscovery.cpp
    
    dataLoader.cpp
    graph.cpp
    main.cpp
    statistic.cpp)

install(TARGETS causalDiscovery DESTINATION .)
install(FILES data.csv DESTINATION .)
