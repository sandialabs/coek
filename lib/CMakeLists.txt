#
# CoekProject Libraries
#

message("")
message("Configuring lib/coek build")
add_subdirectory(coek)
set(lib_test_targets "test_coek")
set(lib_doc_targets "doc_coek")

message("")
if(${with_python})
    message("Configuring lib/pycoek build/install")
    add_subdirectory(pycoek)
else()
    message("Ignoring lib/pycoek")
endif()

message("")
if(${with_python})
    message("Configuring lib/poek install")
    #
    # Configuring pip_install_poek here so we don't have CMake logic in a python subdir
    #
    add_custom_target(pip_install_poek ALL
        COMMAND ${Python_EXECUTABLE} -m pip install -e ${CMAKE_CURRENT_SOURCE_DIR}/poek
        )
    #
    # Targets for poek testing
    #
    if(${with_tests})
        list(APPEND lib_test_targets test_poek test_pyomo test_coek_utils)
        add_custom_target(test_poek
            COMMAND ${Python_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/poek/poek
            )
        add_custom_target(test_coek_utils
            COMMAND ${Python_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/coek_utils/coek_utils
            )
        add_custom_target(test_pyomo
            COMMAND ${Python_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/pyomo_coek/pyomo_coek
            )
    endif()
    #
    # Targets for poek documentation
    #
    if(${with_docs})
        #
        # SPHINX
        #
        set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/poek/doc)
        set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/poek/doc/sphinx)
        set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)
        set(SPHINX_DEPENDS
                    ${SPHINX_SOURCE}/index.rst
                    ${SPHINX_SOURCE}/bibliography.rst
                    ${SPHINX_SOURCE}/constraint.rst
                    ${SPHINX_SOURCE}/examples.rst
                    ${SPHINX_SOURCE}/expression_fn.rst
                    ${SPHINX_SOURCE}/expression.rst
                    ${SPHINX_SOURCE}/models.rst
                    ${SPHINX_SOURCE}/parameter.rst
                    ${SPHINX_SOURCE}/reference.rst
                    ${SPHINX_SOURCE}/variable.rst)
        file(MAKE_DIRECTORY ${SPHINX_BUILD})

        add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                  COMMAND
                    ${SPHINX_EXECUTABLE} -b html
                    ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  DEPENDS ${SPHINX_DEPENDS}
                  MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                  COMMENT "Generating documentation with Sphinx")

        add_custom_target(poek_html DEPENDS ${SPHINX_INDEX_FILE})
        #add_custom_target(poek_pdf DEPENDS ${LATEX_BUILD}/poek.pdf)
        add_custom_target(doc_poek DEPENDS poek_html)
        list(APPEND lib_doc_targets "doc_poek")
    endif()
else()
    message("Ignoring lib/poek")
endif()

message("")
if(${with_python})
    message("Configuring lib/pyomo_coek install")
    add_custom_target(pip_install_pyomo_coek ALL
        COMMAND ${Python_EXECUTABLE} -m pip install -e ${CMAKE_CURRENT_SOURCE_DIR}/pyomo_coek
        )
    add_dependencies(pip_install_pyomo_coek pip_install_poek)
else()
    message("Ignoring lib/pyomo_coek")
endif()

message("")
if(${with_python})
    message("Configuring lib/coek_utils install")
    add_custom_target(pip_install_coek_utils ALL
        COMMAND ${Python_EXECUTABLE} -m pip install -e ${CMAKE_CURRENT_SOURCE_DIR}/coek_utils
        )
else()
    message("Ignoring lib/coek_utils")
endif()

#
# Lib test targets
#
if(${with_tests})
  add_custom_target(test_lib DEPENDS ${lib_test_targets})
endif()

#
# Lib doc targets
#
if(${with_docs})
  add_custom_target(doc_lib DEPENDS ${lib_doc_targets})
endif()
