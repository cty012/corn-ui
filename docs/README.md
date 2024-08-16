# Corn-UI: Official UI library for Corn Game Engine
[![License: MIT](https://img.shields.io/badge/license-MIT-yellow.svg)](https://github.com/cty012/corn/blob/main/LICENSE)
[![Version](https://img.shields.io/badge/version-1.0.0-red.svg)](https://github.com/cty012/corn)

Corn-UI allows developers to create user interfaces for their games using web-like languages.

## Features
- Compatible with Windows, macOS, and Linux
- XML parsing for UI widgets
- Corn-CSS specifically designed for styling UI widgets
- JavaScript (ES6) support for dynamic effects

## Prerequisites
- MSVC (Windows) / MinGW-w64 13.1.0 (Windows) / Clang (macOS) / GCC (Linux)
- [Corn Game Engine](https://github.com/cty012/corn)
- (Optional) [CMake](https://cmake.org/): Only if building from the source. Make sure it is in PATH.
- (Optional) [Conan](https://conan.io/): Only if building from the source. Make sure it is in PATH.
- (Optional) [JSON for Modern C++](https://json.nlohmann.me/): Only if building from the source (installed in conan).
- (Optional) [libxml2](https://gitlab.gnome.org/GNOME/libxml2): Only if building from the source (installed in conan).
- (Optional) [QuickJS](https://bellard.org/quickjs/): Only if building from the source (installed in conan).
- (Optional) [GoogleTest](https://github.com/google/googletest): Only if building the test cases (installed in conan).

## Getting Started

To begin, clone the repository to your local machine using:
```shell
git clone https://github.com/cty012/corn-ui.git
cd corn-ui
```

### Prerequisites
Ensure you have `CMake` and `Conan` installed and added to your PATH.
Check their respective websites for installation and setup instructions.

### Build Instructions

1. Follow the [instructions](https://github.com/cty012/corn) to build and install the Corn Game Engine. Retrieve the
   library in the `dist` folder and place it in the `deps` of this project.

2. Run the following command, replacing `<profile>` with your own Conan profile:

   ```shell
   conan install . -pr=<profile> -of=conan/<profile> -b=missing
   cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=conan/<profile>/conan_toolchain.cmake
   cmake --build build --config Release
   ```

   After building, the compiled dynamic library will be available in the `build` directory.

## Installation
Install the library by running the following command in the project root:
```shell
cmake --install build
```
This will install the library into the `dist` folder.
For both debug and release builds, repeat the command with the respective build folders.

## Documentation (currently unavailable)
The documentation of this project is generated using [Doxygen](https://www.doxygen.nl/).
To generate the documentation, install Doxygen and run the following command:
```shell
cd docs && doxygen
```

## Acknowledgments
- [CMake](https://cmake.org/) - Licensed under [3-Clause BSD License](https://cmake.org/licensing/)
- [Conan](https://conan.io/) - Licensed under [MIT License](https://github.com/conan-io/conan/blob/develop2/LICENSE.md)
- [JSON for Modern C++](https://json.nlohmann.me/) - Licensed under [MIT License](https://json.nlohmann.me/home/license/)
- [libxml2](https://gitlab.gnome.org/GNOME/libxml2) - Licensed under [MIT License](https://gitlab.gnome.org/GNOME/libxml2/-/blob/master/Copyright)
- [QuickJS](https://bellard.org/quickjs/) - Licensed under [MIT License](https://github.com/bellard/quickjs/blob/master/LICENSE)
- [GoogleTest](https://google.github.io/googletest) - Licensed under [BSD 3-Clause License](https://github.com/google/googletest/blob/main/LICENSE)
