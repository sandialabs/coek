if(with_python)
    execute_process(COMMAND ${Python_EXECUTABLE} -m black --version
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        RESULT_VARIABLE BLACKFORMATRESULT
        OUTPUT_VARIABLE BLACKFORMATVERSION
        ERROR_VARIABLE BLACKFORMATERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(BLACKFORMATRESULT EQUAL 0)
        #
        # Successfully got the version information from the python 'black' package
        #
        message(STATUS "Found ${BLACKFORMATVERSION}")

        file(GLOB_RECURSE ALL_PYTHON_SOURCE_FILES
            (${CMAKE_SOURCE_DIR}/lib/*.py) | (${CMAKE_SOURCE_DIR}/test/*.py)
        )

        if ("${ALL_PYTHON_SOURCE_FILES}" STREQUAL "")
            message("WARNING: Python formatting targets not setup because no source files found!")
        else()

            add_custom_target(format-py
                COMMAND python
                    -m black
                    -l 100
                    ${ALL_PYTHON_SOURCE_FILES}
                COMMENT "Format all source files. This may take a while..."
            )

            add_custom_target(format-py-check
                # Use ! to negate the result for correct output
                COMMAND !
                    python
                    -m black
                    -l 100
                    --check
                    ${ALL_PYTHON_SOURCE_FILES}
                COMMENT "Checking python format changes."

            )
        endif()

    else()
        message(SEND_ERROR "Cannot retrieve version information for Python black package")
    endif()

endif()

