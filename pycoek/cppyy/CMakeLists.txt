cmake_minimum_required(VERSION 3.7)

execute_process(COMMAND cling-config --cmake OUTPUT_VARIABLE CPPYY_MODULE_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# Make sure this is set to something like:
# ~/.local/lib/python2.7/site-packages/cpyy_backend/cmake 
#message("CPYY_MODULE_PATH: " ${CPPYY_MODULE_PATH})
list(INSERT CMAKE_MODULE_PATH 0 ${CPPYY_MODULE_PATH})

#get_filename_component(CURR_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
#set(ENV{CMAKE_CPPYY_SOURCE_DIR} "${CURR_DIR}")
#set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
#include(GNUInstallDirs)

find_package(Cppyy)


list(APPEND LIB_HEADERS 
#    ${CMAKE_SOURCE_DIR}/coek/ast/varray.hpp
#    ${CMAKE_SOURCE_DIR}/coek/api/expression.hpp
#    ${CMAKE_SOURCE_DIR}/coek/api/objective.hpp
#    ${CMAKE_SOURCE_DIR}/coek/api/constraint.hpp
#    ${CMAKE_SOURCE_DIR}/coek/coek_model.hpp
     ${CMAKE_SOURCE_DIR}/coek/coek.hpp
    )
list(APPEND LIB_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}
    )

if(${with_fmtlib})
  list(APPEND OPT_LINK_LIBRARIES fmt)
endif()
if(${with_gurobi})
  LINK_DIRECTORIES($ENV{GUROBI_HOME}/lib)
  list(APPEND OPT_LINK_LIBRARIES gurobi_g++5.2 gurobi90)
endif()
if(${with_ipopt})
  LINK_DIRECTORIES($ENV{IPOPT_HOME}/lib)
  list(APPEND OPT_LINK_LIBRARIES ipopt gfortran)
endif()

#
# Set up the Cppyy bindings generation.
#
cppyy_add_bindings(
   "pycoek_cppyy"
   "${PROJECT_VERSION}"
   "William Hart"
   "whart222@gmail.com"
   URL                  "https://github.com/or-fusion/coek"
   LICENSE              "BSD-3"
   LANGUAGE_STANDARD    "17"
   GENERATE_OPTIONS     -I${CMAKE_SOURCE_DIR}
   #EXTRA_HEADERS        ${CMAKE_SOURCE_DIR}/pycoek/cppyy/cppyy_interface.hpp
   INCLUDE_DIRS         ${LIB_INCLUDE_DIRS}
   H_FILES              ${CMAKE_SOURCE_DIR}/pycoek/cppyy/cppyy_interface.hpp #${LIB_HEADERS}
   LINK_LIBRARIES       coek ${OPT_LINK_LIBRARIES}
)
#NAMESPACES           coek
#SELECTION_XML  ${CMAKE_SOURCE_DIR}/pycoek/cppyy/selection.xml

add_custom_target(pip_install_cppyy
        DEPENDS pycoek_cppyy
        COMMAND pip install --force-reinstall ${PY_WHEEL_FILE}
        COMMAND python -c "import pycoek_cppyy"
        )
