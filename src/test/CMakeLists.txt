
add_executable(engine_test engine_test.cpp)

target_link_libraries(engine_test
    PRIVATE
    engine
    gtest_maind gmock_maind gmockd gtestd)

add_test(NAME engine_test COMMAND engine_test)

