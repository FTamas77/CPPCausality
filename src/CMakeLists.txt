add_executable(causalDiscovery 
    causalDiscovery.cpp
    dataLoader.cpp
    graph.cpp
    main.cpp
    statistic.cpp)

target_link_libraries(causalDiscovery ${CONAN_LIBS})

install(TARGETS causalDiscovery DESTINATION .)
install(FILES data.csv DESTINATION .)
