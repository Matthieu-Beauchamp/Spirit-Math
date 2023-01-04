# Spirit-Module


## Key features
-

## Installation

Requires:
- Cmake 3.13
- C++20 compiler 
  - tested on GCC 12.1

Currently, cmake installation is not supported.
The recommended approach is to use Spirit-Module as a submodule to your project.

Download:
```sh
cd MyProject/libs/
git submodule add https://github.com/Matthieu-Beauchamp/Spirit-Module
git submodule update --init --recursive
```

In MyProject/CMakeLists.txt :
```cmake
add_subdirectory(libs/Spirit-Module)

add_executable(myExe main.cpp)
target_link_libraries(myExe spirit-module)
```

## Reference
To build documentation: 
```cmake
set(SPIRIT_MODULE_BUILD_DOCS TRUE)
add_subdirectory(libs/Spirit-Module)
```
will reveal a Spirit-Module-docs CMake target
that will generate Doxygen html documentation in Spirit-Module/docs/Out/html/index.html
when built.

There are some examples of typical usage in the examples folder.

# Licenses
While Spirit-Module's code is licensed under the zlib license, it uses the following
modules which have differing licenses:

-

# Developpement
Developpement should be done through the Spirit super-project at 
https://github.com/Matthieu-Beauchamp/Spirit since it includes
the .clang-format and allows for easy toggling of internal targets per module.

Issues for modules should still be posted per-module and not on the 
Spirit repository.