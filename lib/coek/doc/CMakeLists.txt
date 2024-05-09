find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)
find_package(LATEX COMPONENTS PDFLATEX)

#
# DOXYGEN
#
# Find all the public headers
get_target_property(CAT_PUBLIC_HEADER_DIR coek INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE CAT_PUBLIC_HEADERS ${CAT_PUBLIC_HEADER_DIR}/*.hpp)

set(DOXYGEN_INPUT_DIR "${PROJECT_SOURCE_DIR}/coek ${PROJECT_SOURCE_DIR}/coek/api")
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#
# SPHINX
#
set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)
set(SPHINX_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/install.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/overview.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/reference.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/parameter.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/variable.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/expression.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/objective.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/constraint.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/api.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/api_sets.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/api_models.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/components_elementary.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/components_indexed.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/components_sequence.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/expression_fn.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/models.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/examples.rst
            ${CMAKE_CURRENT_SOURCE_DIR}/bibliography.rst)

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Create output directory
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR}) 

# This will be the main output of our command
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${CAT_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating docs")

add_custom_target(coek_doxygen DEPENDS ${DOXYGEN_INDEX_FILE})


# Regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                  COMMAND
                    ${SPHINX_EXECUTABLE} -b html
                    # Tell Breathe where to find the Doxygen output
                    -Dbreathe_projects.Coek=${DOXYGEN_OUTPUT_DIR}/xml
                    ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  DEPENDS ${SPHINX_DEPENDS} ${DOXYGEN_INDEX_FILE}
                  MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                  COMMENT "Generating documentation with Sphinx")


if(LATEX_PDFLATEX_FOUND)
    set(LATEX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/latex)
    file(MAKE_DIRECTORY ${LATEX_BUILD}) 

    add_custom_command(
        OUTPUT ${LATEX_BUILD}/coek.tex
        COMMAND ${SPHINX_EXECUTABLE} -b latex ${CMAKE_CURRENT_SOURCE_DIR} ${LATEX_BUILD}
        DEPENDS ${SPHINX_DEPENDS} ${DOXYGEN_INDEX_FILE})

    add_custom_command(
        OUTPUT "${LATEX_BUILD}/coek.pdf"
        # Three times' the charm for PdfLaTeX to get all xrefs right
        COMMAND "${PDFLATEX_COMPILER}" "coek.tex"
        COMMAND "${PDFLATEX_COMPILER}" "coek.tex"
        COMMAND "${PDFLATEX_COMPILER}" "coek.tex"
        WORKING_DIRECTORY "${LATEX_BUILD}"
        DEPENDS "${LATEX_BUILD}/coek.tex")

    list(APPEND TARGET_DEPENDS "${LATEX_BUILD}/coek.pdf")
else()
    message(WARNING "No PdfLaTeX found. PDF output not available.")
endif()

# Nice named target so we can run the job easily
add_custom_target(coek_html DEPENDS ${SPHINX_INDEX_FILE})
add_custom_target(coek_pdf DEPENDS ${LATEX_BUILD}/coek.pdf)

#add_custom_target(doc_coek DEPENDS coek_doxygen coek_html coek_pdf)
add_custom_target(doc_coek DEPENDS coek_doxygen coek_html)

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})

