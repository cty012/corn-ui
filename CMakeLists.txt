cmake_minimum_required(VERSION 3.22)

# CMake cache variables
option(BUILD_EXAMPLES "Build the example programs in the `examples/` folder." ON)
option(BUILD_TESTS "Build the test cases in the `test/` folder." OFF)

if (BUILD_TESTS)
    enable_testing()
    set(VCPKG_MANIFEST_FEATURES "${VCPKG_MANIFEST_FEATURES};tests")
endif ()

# Project settings
project(corn-ui VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(DEPS_PATH "${CMAKE_SOURCE_DIR}/deps")
list(APPEND CMAKE_PREFIX_PATH "${DEPS_PATH}")

if (APPLE)
    set(CMAKE_INSTALL_RPATH "@loader_path;@loader_path/../lib;@loader_path/../lib/external;@loader_path/../Frameworks")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_INSTALL_RPATH "$ORIGIN;$ORIGIN/../lib;$ORIGIN/../lib/external;$ORIGIN/../Frameworks")
endif ()

# Output directories
set(CORNUI_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")
set(EXAMPLES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/examples")
set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/tests")

# Platform
if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM "win64")
    else()
        set(PLATFORM "win32")
    endif()
elseif(APPLE)
    if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
        set(PLATFORM "macos-arm64")
    elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
        set(PLATFORM "macos-x64")
    else()
        set(PLATFORM "macos")
    endif()
elseif(UNIX)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
        set(PLATFORM "linux-x64")
    else()
        set(PLATFORM "linux")
    endif()
else()
    set(PLATFORM "unknown")
endif()

# Compiler flags
#if(CMAKE_BUILD_TYPE STREQUAL "Release")
#    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
#endif()
if (MSVC)
    add_compile_options(/utf-8 /W4 /WX)
    add_compile_options("$<$<CONFIG:Release>:/Ox>")
    add_compile_options("$<$<CONFIG:Debug>:/Zi>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
else ()
    add_compile_options(-Wall -Wextra -Werror)
    add_compile_options("$<$<CONFIG:Release>:-O3>")
    add_compile_options("$<$<CONFIG:Debug>:-g>")
endif ()

# For static analysis and linting
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Corn UI target
file(GLOB_RECURSE HEADERS
        "${CMAKE_SOURCE_DIR}/include/*.h"
        "${CMAKE_SOURCE_DIR}/include/*.cpp"
)
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.h"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_library(cornui SHARED ${HEADERS} ${SOURCES})
set_target_properties(cornui PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CORNUI_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CORN_OUTPUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CORN_OUTPUT_DIR}"
        DEBUG_POSTFIX -d
)
set(CORNUI_OUTPUT_PATH $<TARGET_FILE:cornui>)
target_include_directories(cornui PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Link with corn
find_package(Corn REQUIRED)
target_link_libraries(cornui PRIVATE Corn::Corn)

# Link with nlohmann_json
find_package(nlohmann_json REQUIRED)
target_link_libraries(cornui PUBLIC nlohmann_json::nlohmann_json)

# Link with libxml2
find_package(libxml2 REQUIRED)
target_link_libraries(cornui PRIVATE LibXml2::LibXml2)

# Link with duktape
find_package(duktape CONFIG REQUIRED)
target_include_directories(cornui PRIVATE ${DUKTAPE_INCLUDE_DIRS})
target_link_libraries(cornui PRIVATE ${DUKTAPE_LIBRARY})

# Build examples
if (BUILD_EXAMPLES)
    add_subdirectory("examples")
endif ()

# Build test cases
if (BUILD_TESTS)
    add_subdirectory("test")
endif ()

# Install
set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist/cornui-${PROJECT_VERSION}-${PLATFORM}")
install(TARGETS cornui
        RUNTIME DESTINATION "bin"
        LIBRARY DESTINATION "bin"
        ARCHIVE DESTINATION "lib"
)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/cornui" DESTINATION "include")
get_target_property(nlohmann_json_INCLUDE_DIRS nlohmann_json::nlohmann_json INTERFACE_INCLUDE_DIRECTORIES)
list(GET nlohmann_json_INCLUDE_DIRS 0 nlohmann_json_INCLUDE_DIR)
install(DIRECTORY "${nlohmann_json_INCLUDE_DIR}/nlohmann" DESTINATION "include")

install(PROGRAMS "${Corn_DYNAMIC_LIBRARY}" DESTINATION "lib/external")
foreach (external_lib IN LISTS Corn_EXTERNAL_LIBRARIES)
    install(PROGRAMS "${external_lib}" DESTINATION "lib/external")
endforeach ()

configure_file(cornui-config.cmake.in cornui-config.cmake @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cornui-config.cmake" DESTINATION "cmake")

install(FILES "${CMAKE_SOURCE_DIR}/LICENSE" DESTINATION ".")
