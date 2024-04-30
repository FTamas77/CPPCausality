[Debug build maybe needed](https://discourse.cmake.org/t/findboost-cmake-seems-to-work-but-adds-no-include-folders-to-compile-line-when-linked-against/7923)

**It says:** "Ultimately I discovered the issue after viewing the INTERFACE_INCLUDE_DIRECTORIES instead of the INCLUDE_DIRECTORIES. This was set properly, but had a generator expression that set the value to empty unless the CONFIG was Release. So I needed to run my conan install with a profile that specifically called out Debug in order to match my project’s build settings."

**The applied profile:**

```cmake
[CMake] ======== Input profiles ========
1> [CMake] Profile host:
1> [CMake] [settings]
1> [CMake] arch=x86_64
1> [CMake] build_type=Release
1> [CMake] compiler=msvc
1> [CMake] compiler.cppstd=17
1> [CMake] compiler.runtime=dynamic
1> [CMake] compiler.runtime_type=Release
1> [CMake] compiler.version=193
1> [CMake] os=Windows
1> [CMake]
1> [CMake] Profile build:
1> [CMake] [settings]
1> [CMake] arch=x86_64
1> [CMake] build_type=Release
1> [CMake] compiler=msvc
1> [CMake] compiler.cppstd=17
1> [CMake] compiler.runtime=dynamic
1> [CMake] compiler.runtime_type=Release
1> [CMake] compiler.version=193
1> [CMake] os=Windows
1> [CMake]
1> [CMake]
```
