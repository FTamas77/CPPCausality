cmake_minimum_required (VERSION 3.8)

project ("engine")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GenerateExportHeader)

add_library(engine SHARED "engine.cpp")

target_include_directories(engine PRIVATE
    "C:/Users/Tamas.Fekete/AppData/Local/Programs/Python/Python310/Include"
    "D:/Projects/Causality/causalityEnv3.10/lib/site-packages/pybind11/include"
    "C:/Users/Tamas.Fekete/AppData/Local/Programs/Python/Python310/Include"
)

target_link_libraries(engine PRIVATE
    "C:/Users/Tamas.Fekete/AppData/Local/Programs/Python/Python310/libs/python310.lib"
)

set_target_properties(engine PROPERTIES SUFFIX ".pyd")

GENERATE_EXPORT_HEADER(engine
    BASE_NAME engine
    EXPORT_MACRO_NAME engine_EXPORT
    EXPORT_FILE_NAME engine_Export.h
    STATIC_DEFINE engine_BUILT_AS_STATIC
)

install()
