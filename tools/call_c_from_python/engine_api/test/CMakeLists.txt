add_executable(engine_api_test engine_api_test.cpp)

target_include_directories(engine_api_test PRIVATE 
    ${HEADER_ROOT_DIR}
    ${CMAKE_SOURCE_DIR}/OpenCL-Headers)

target_link_libraries(engine_api_test PRIVATE
    engine_api
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main)

if(UNIX)
    target_link_libraries(engine_api_test PRIVATE pthread)
endif()

add_test(NAME engine_api_test COMMAND engine_api_test)

