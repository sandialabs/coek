cmake_minimum_required(VERSION 3.7)

execute_process(COMMAND cling-config --cmake OUTPUT_VARIABLE CPPYY_MODULE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# Make sure this is set to something like:
# ~/.local/lib/python2.7/site-packages/cpyy_backend/cmake 
message("   CPPYY_MODULE_PATH: " ${CPPYY_MODULE_PATH})
list(INSERT CMAKE_MODULE_PATH 0 ${CPPYY_MODULE_PATH})

#get_filename_component(CURR_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
#set(ENV{CMAKE_CPPYY_SOURCE_DIR} "${CURR_DIR}")
#set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
#include(GNUInstallDirs)

if (fmtlib_available)
    find_package(fmt REQUIRED)
endif()
find_package(Cppyy)


list(APPEND LIB_HEADERS ${CMAKE_SOURCE_DIR}/coek/coek.hpp)
list(APPEND LIB_INCLUDE_DIRS ${CMAKE_SOURCE_DIR})
message("HERE ${LIB_INCLUDE_DIRS}")

#
# Set up the Cppyy bindings generation.
#
cppyy_add_bindings(
   "pycoek_cppyy"
   "${COEK_VERSION}"
   "William Hart"
   "whart222@gmail.com"
   URL                  "https://github.com/or-fusion/coek"
   LICENSE              "BSD-3"
   LANGUAGE_STANDARD    "17"
   GENERATE_OPTIONS     "-I${CMAKE_SOURCE_DIR}"
   INCLUDE_DIRS         "${LIB_INCLUDE_DIRS}"
   LINKDEFS             "${CMAKE_SOURCE_DIR}/pycoek/cppyy/linkdefs.hpp"
   README_FILE          "${CMAKE_SOURCE_DIR}/pycoek/cppyy/README.rst"
   LINK_LIBRARIES       coek ${coek_link_libraries}
   NAMESPACES           "coek"
   H_FILES              ${CMAKE_SOURCE_DIR}/coek/coek_simple.hpp
                        ${CMAKE_SOURCE_DIR}/pycoek/cppyy/cppyy_interface.hpp
                        ${CMAKE_SOURCE_DIR}/coek/api/expression.hpp
                        ${CMAKE_SOURCE_DIR}/coek/api/variable_array.hpp
)
                        #${CMAKE_SOURCE_DIR}/coek/api/constraint.hpp
                        #${CMAKE_SOURCE_DIR}/coek/api/objective.hpp
                        #${CMAKE_SOURCE_DIR}/coek/api/intrinsic_fn.hpp
#   H_FILES              "${CMAKE_SOURCE_DIR}/coek/coek.hpp;${CMAKE_SOURCE_DIR}/pycoek/cppyy/cppyy_interface.hpp"
   #EXTRA_HEADERS        ${CMAKE_SOURCE_DIR}/pycoek/cppyy/cppyy_interface.hpp
   #SELECTION_XML        ${CMAKE_SOURCE_DIR}/pycoek/cppyy/selection.xml

add_custom_target(pip_install_cppyy
        DEPENDS pycoek_cppyy
        COMMAND pip install --force-reinstall ${PY_WHEEL_FILE}
        COMMAND python -c "import pycoek_cppyy"
        )
