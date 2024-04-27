add_executable(causalDiscovery 
    causalDiscovery.cpp
    dataGenerator.cpp
    dataLoader.cpp
    dataset.cpp
    graph.cpp
    main.cpp
)

install(TARGETS causalDiscovery DESTINATION .)
install(FILES data.csv DESTINATION .)
