
## Use C++ in Python

https://learn.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022

## Temporally removed from root CMakeLists

```cmake
    if(MSVC)
        include(conanbuildinfo.cmake)
    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        include(/home/umc/CPPCausality/conanbuildinfo.cmake)

        # remote pybind11
        find_package(Python3 REQUIRED)
        set(pybind11_DIR /home/umc/.local/lib/python3.9/site-packages/pybind11)
        find_package(pybind11 REQUIRED HINTS "${pybind11_DIR}")
    endif()
        conan_basic_setup()

    # needed for Python
    include(GenerateExportHeader)
```
