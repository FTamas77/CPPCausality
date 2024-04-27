## Unit test
In case of conan 2.0 the wrapper to CMake is this:
https://github.com/conan-io/cmake-conan

## Use C++ in Python
https://learn.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022

## Opencl repo
https://github.com/KhronosGroup/OpenCL-Headers

**Intel is based on this:**
https://www.intel.com/content/www/us/en/developer/articles/tool/opencl-drivers.html

**Installed from here:**
https://www.intel.com/content/www/us/en/developer/articles/technical/intel-cpu-runtime-for-opencl-applications-with-sycl-support.html

**It says it is installed here:**
c:\Program Files (x86)\Common Files\Intel\Shared Libraries\

NVidia quadro t2000:
https://www.nvidia.com/content/dam/en-zz/Solutions/design-visualization/quadro-product-literature/NV_DS_QUADRO_2000_US_LR.pdf

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
