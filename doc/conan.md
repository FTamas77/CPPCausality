# Conan Package Manager Configuration

This document describes the configuration used for Conan package manager in this project.

## Conan Profile

A Conan profile contains settings, options, environment variables, and build requirements that will be used for building packages.

```
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

> Note: The `[conf]` section may be optional if you're using the default VS version.

## Global Configuration

The global configuration affects all Conan operations regardless of the profile being used. 
These settings can be found in the Conan configuration file.

```
# Storage path for downloaded packages and binaries
core.cache:storage_path = D:/conan_data

# Visual Studio path (if needed for specific project requirements)
# tools.microsoft.msbuild:installation_path = C:/Program Files/Microsoft Visual Studio/2022/Professional
```

## Usage

### Creating a Profile File

Save the profile configuration to a file (e.g., `windows_debug.profile`), then reference it during Conan commands:

```
conan install . --profile=windows_debug
```

### Setting Global Configuration

Use the following command to set global configuration options:

```
conan config set core.cache:storage_path=D:/conan_data
```

## Common Commands

- List profiles: `conan profile list`
- List configuration options: `conan config list`
- Install dependencies: `conan install . --profile=<profile_name>`
- Build package: `conan build .`
