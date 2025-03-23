# Using C++ in Python

This document describes how to integrate C++ code with Python in the CPPCausality project.

## Why Use C++ with Python?

Combining C++ and Python offers several advantages:
- **Performance**: Use C++ for computationally intensive operations
- **Reuse**: Leverage existing C++ libraries in Python applications
- **Integration**: Access system resources and hardware directly through C++

## Integration Methods

We use [pybind11](https://github.com/pybind/pybind11) to create Python bindings for C++ code. For more information, see the [Microsoft documentation](https://learn.microsoft.com/en-us/visualstudio/python/working-with-c-cpp-python-in-visual-studio?view=vs-2022).

## CMake Configuration

The following configuration was temporarily removed from the root CMakeLists.txt but should be used when integrating Python:

```cmake
if(MSVC)
    include(conanbuildinfo.cmake)
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    include(/home/umc/CPPCausality/conanbuildinfo.cmake)

    # Configure pybind11
    find_package(Python3 REQUIRED)
    set(pybind11_DIR /home/umc/.local/lib/python3.9/site-packages/pybind11)
    find_package(pybind11 REQUIRED HINTS "${pybind11_DIR}")
endif()
    conan_basic_setup()

# Needed for Python integration
include(GenerateExportHeader)
```

## Setup Instructions

1. Install the required dependencies:
   ```bash
   pip install pybind11
   ```

2. Add the CMake configuration above to your root CMakeLists.txt

3. Create a Python module by adding the following to your component's CMakeLists.txt:
   ```cmake
   pybind11_add_module(your_module_name your_binding_code.cpp)
   target_link_libraries(your_module_name PRIVATE your_cpp_library)
   ```

## Usage Example

```python
import your_module_name

# Now you can use your C++ functions
result = your_module_name.your_function(args)
```

## Troubleshooting

- Check Python and pybind11 versions are compatible
- Ensure paths in CMake configuration match your environment
- Verify all required libraries are linked correctly
