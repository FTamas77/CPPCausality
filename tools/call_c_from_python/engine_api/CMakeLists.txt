﻿add_library(engine_api SHARED
    engine_api.cpp
    engine_api.h
    engine_api_bindings.h)

if(MSVC)
    target_include_directories(engine_api PUBLIC
        "C:/Users/Tamas.Fekete/AppData/Local/Programs/Python/Python310/Include"
        "D:/Projects/Causality/causalityEnv3.10/lib/site-packages/pybind11/include")

    target_link_libraries(engine_api PUBLIC
        "C:/Users/Tamas.Fekete/AppData/Local/Programs/Python/Python310/libs/python310.lib")

elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    target_include_directories(engine_api PUBLIC
        "${pybind11_DIR}/include"
        "/usr/include/python3.9")

    # https://pybind11.readthedocs.io/en/stable/compiling.html#embedding-the-python-interpreter 
    target_link_libraries(engine_api PUBLIC pybind11::embed ${CMAKE_THREAD_LIBS_INIT})
endif()

set_target_properties(engine_api PROPERTIES SUFFIX ".pyd")

GENERATE_EXPORT_HEADER(engine_api
    BASE_NAME engine_api
    EXPORT_MACRO_NAME engine_api_EXPORT
    EXPORT_FILE_NAME engine_api_Export.h
    STATIC_DEFINE engine_api_BUILT_AS_STATIC
)

if(RUN_TESTS)
    add_subdirectory(test)
endif()

