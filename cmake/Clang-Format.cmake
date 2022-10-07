# Adapted from Clang-Format.cmake logic defined here:
#
# https://clangformat.com/
# https://zed0.co.uk/clang-format-configurator/
# https://clang.llvm.org/docs/ClangFormatStyleOptions.html

# // clang-format off
# void unformatted_code:
# // clang-format on

find_program(CLANGFORMAT clang-format NAMES clang-format clang-format-9 clang-format-10 clang-format-11)

if(CLANGFORMAT)
    execute_process(COMMAND ${CLANGFORMAT} --version
        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
        RESULT_VARIABLE CLANGFORMATRESULT
        OUTPUT_VARIABLE CLANGFORMATVERSION
        ERROR_VARIABLE CLANGFORMATERROR
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(CLANGFORMATRESULT EQUAL 0)
        #
        # Successfully got the version information from clang-format executable
        #
        message(STATUS "Found ${CLANGFORMATVERSION}: " ${CLANGFORMAT})

        file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
            (${CMAKE_SOURCE_DIR}/lib/*.[ch]pp) | (${CMAKE_SOURCE_DIR}/test/*.[ch]pp)
        )

        if ("${ALL_CXX_SOURCE_FILES}" STREQUAL "")
            message("WARNING: C++ formatting targets not setup because no source files found!")
        else()

            add_custom_target(format-cpp
                COMMAND ${CLANGFORMAT}
                    -i
                    -style=file
                    -fallback-style=none
                    -verbose
                    ${ALL_CXX_SOURCE_FILES}
                SOURCES "${CMAKE_SOURCE_DIR}/.clang-format"
                COMMENT "Format all source files. This may take a while..."
            )

            #set_target_properties(Format PROPERTIES
            #    FOLDER "${META_PROJECT_NAME}/Tools"
            #)

            add_custom_target(format-cpp-check
                # Use ! to negate the result for correct output
                COMMAND !
                    ${CLANGFORMAT}
                    -style=file
                    -output-replacements-xml
                    -fallback-style=none
                    -verbose
                    ${ALL_CXX_SOURCE_FILES}
                    | grep -q "Replacement offset"
                SOURCES "${CMAKE_SOURCE_DIR}/.clang-format"
                COMMENT "Checking clang-format changes."

            )

            #set_target_properties(Format-Check PROPERTIES
            #    FOLDER "${META_PROJECT_NAME}/Tools"
            #)

            #add_custom_target(format-cpp-dry
            #    COMMAND ${CLANGFORMAT}
            #        -verbose
            #        -style=file
            #        -dry-run
            #        -fallback-style=none
            #        ${ALL_CXX_SOURCE_FILES}
            #    SOURCES "${CMAKE_SOURCE_DIR}/.clang-format"
            #    COMMENT "Running clang-format in dry mode."
            #)

            #set_target_properties(Format-Dry PROPERTIES
            #    FOLDER "${META_PROJECT_NAME}/Tools"
            #)
        endif()

    else()
        message(SEND_ERROR "clang-format found but cannot retrieve version information")
    endif()

else()
    message(STATUS "clang-format requested but executable not found")
endif()

