cmake_minimum_required(VERSION 3.22)
project(corn-ui)

set(CMAKE_CXX_STANDARD 20)
set(DEPS_PATH "${CMAKE_SOURCE_DIR}/deps")
list(APPEND CMAKE_PREFIX_PATH "${DEPS_PATH}")

option(BUILD_EXAMPLES "Build the example programs in the `examples/` folder." ON)
option(BUILD_TESTS "Build the test cases in the `test/` folder." OFF)

if (MSVC)
    add_compile_options(/utf-8 /W4 /WX)
    add_compile_options("$<$<CONFIG:Release>:/O2>")
    add_compile_options("$<$<CONFIG:Debug>:/Zi>")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
else ()
    add_compile_options(-Wall -Wextra -Werror)
    add_compile_options("$<$<CONFIG:Release>:-O3>")
    add_compile_options("$<$<CONFIG:Debug>:-g>")
endif ()

set(CORNUI_OUTPUT_DIR "${CMAKE_BINARY_DIR}/lib")
set(EXAMPLES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/examples")
set(TEST_OUTPUT_DIR "${CMAKE_BINARY_DIR}/tests")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE HEADERS
        "${CMAKE_SOURCE_DIR}/include/*.h"
        "${CMAKE_SOURCE_DIR}/include/*.cpp"
)
file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.h"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_library(cornui SHARED ${HEADERS} ${SOURCES})
set_target_properties(cornui PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CORNUI_OUTPUT_DIR}")
set(CORNUI_OUTPUT_PATH $<TARGET_FILE:cornui>)
target_include_directories(cornui PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Link with corn
find_package(Corn REQUIRED)
target_link_libraries(cornui Corn::Corn)
#target_link_libraries(cornui PRIVATE Corn::Corn)

# Link with libxml2
find_package(LibXml2 REQUIRED)
target_link_libraries(cornui LibXml2::LibXml2)
#target_link_libraries(cornui PRIVATE LibXml2::LibXml2)

# Link with duktape
find_package(Duktape CONFIG REQUIRED)
# target_include_directories(cornui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${DUKTAPE_INCLUDE_DIRS})
target_link_libraries(cornui ${DUKTAPE_LIBRARY})
#target_link_libraries(cornui PRIVATE ${DUKTAPE_LIBRARY})

# Build examples
if (BUILD_EXAMPLES)
    add_subdirectory("examples")
endif ()

# Build test cases
if (BUILD_TESTS)
    add_subdirectory("test")
endif ()

# Temp: build main
add_executable(main examples/main.cpp)
target_link_libraries(main PRIVATE cornui)
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include")
