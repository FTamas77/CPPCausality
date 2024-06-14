# Conan profile

conf is probably not needed as it is the default value

```[settings]
[settings]
arch=x86_64
build_type=Debug
compiler=msvc
compiler.cppstd=20
compiler.runtime=dynamic
compiler.version=194
os=Windows

[conf]
tools.microsoft.msbuild:vs_version=17
```

# Global configuration

```
# Core configuration (type 'conan config list' to list possible values)
# e.g, for CI systems, to raise if user input would block
# core:non_interactive = True
# some tools.xxx config also possible, though generally better in profiles
# tools.android:ndk_path = my/path/to/android/ndk

# Probalby this also not needed:
tools.microsoft.msbuild:installation_path= C:/Program Files/Microsoft Visual Studio/2022/Professional

core.cache:storage_path = D:/conan_data
```
