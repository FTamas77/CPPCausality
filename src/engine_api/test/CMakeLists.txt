
add_executable(engine_api_test engine_api_test.cpp)

target_include_directories(engine_api_test PRIVATE 
       ${HEADER_ROOT_DIR})

target_link_libraries(engine_api_test
    PRIVATE
    engine_api
    gtest_maind gmock_maind gmockd gtestd)

add_test(NAME engine_api_test COMMAND engine_api_test)

