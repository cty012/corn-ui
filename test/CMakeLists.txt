find_package(GTest REQUIRED)

file(GLOB_RECURSE TEST_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(cornui_test ${TEST_HEADERS} ${TEST_SOURCES})
set_target_properties(cornui_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIR}")
target_include_directories(cornui_test PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/common"
        "${CMAKE_SOURCE_DIR}/include"
)
target_link_libraries(cornui_test PRIVATE GTest::gtest GTest::gtest_main Corn::Corn cornui)

if (NOT ${CORNUI_OUTPUT_DIR} STREQUAL ${TEST_OUTPUT_DIR})
    add_custom_target(cornui_test_dll ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CORNUI_OUTPUT_PATH}"
                "${TEST_OUTPUT_DIR}"
            COMMENT "Copied cornui DLL to test output directory"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${Corn_RUNTIME_DIR}"
                "${TEST_OUTPUT_DIR}"
            COMMENT "Copied corn DLL to test output directory"
    )
    add_dependencies(cornui_test cornui_test_dll)
endif ()

# Copy resources folder
file(GLOB_RECURSE RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")
add_custom_target(cornui_test_resources ALL
        COMMAND ${CMAKE_COMMAND} -E echo "Copying resources..."
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${TEST_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E remove_directory
        "${TEST_OUTPUT_DIR}/resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "${TEST_OUTPUT_DIR}/resources"
        SOURCES ${RESOURCES}
)
add_dependencies(cornui_test cornui_test_resources)
