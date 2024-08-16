macro(run_conan)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
        message(STATUS "Running conan install")
        execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR} --output-folder=${CMAKE_BINARY_DIR}/conan --build=missing
                RESULT_VARIABLE CONAN_RESULT
                OUTPUT_VARIABLE CONAN_OUTPUT)
        if(NOT CONAN_RESULT EQUAL "0")
            message(FATAL_ERROR "Conan install failed: ${CONAN_OUTPUT}")
        endif()
    else()
        message(STATUS "Conan dependencies are already installed")
    endif()
endmacro()
