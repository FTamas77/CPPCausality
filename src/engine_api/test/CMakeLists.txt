add_executable(engine_api_test engine_api_test.cpp)

# TODO: should we use conan package?
target_include_directories(engine_api_test PRIVATE 
       ${HEADER_ROOT_DIR}
       ${CMAKE_SOURCE_DIR}/OpenCL-Headers)

target_link_libraries(engine_api_test PRIVATE
    engine_api
    gtest_maind gmock_maind gmockd gtestd)

if (UNIX)
    target_link_libraries(engine_api_test PRIVATE
    pthread)
endif()


add_test(NAME engine_api_test COMMAND engine_api_test)

