cmake_minimum_required(VERSION 3.7)

set(CMAKE_CPPYY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/pycoek/cppyy)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
include(GNUInstallDirs)

#
# Add our project's cmake dir the the module path. This gives us the
# Cppyy commands and targets.
#
#list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)
find_package(Cppyy)

list(APPEND LIB_HEADERS 
    ${CMAKE_SOURCE_DIR}/coek/ast/varray.hpp
    ${CMAKE_SOURCE_DIR}/coek/api/expression.hpp
    ${CMAKE_SOURCE_DIR}/coek/api/objective.hpp
    ${CMAKE_SOURCE_DIR}/coek/api/constraint.hpp
    ${CMAKE_SOURCE_DIR}/coek/coek_model.hpp
    )
list(APPEND LIB_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}
    )

if(${with_gurobi})
  LINK_DIRECTORIES($ENV{GUROBI_HOME}/lib)
  list(APPEND OPT_LINK_LIBRARIES gurobi_g++5.2 gurobi81)
endif()
if(${with_ipopt})
  LINK_DIRECTORIES($ENV{IPOPT_HOME}/lib)
  list(APPEND OPT_LINK_LIBRARIES ipopt gfortran)
endif()

#
# Set up the Cppyy bindings generation.
#
cppyy_add_bindings(
    "pycoek_cppyy" "${PROJECT_VERSION}" "William Hart" "whart222@gmail.com"
   LICENSE "BSD-3"
   LANGUAGE_STANDARD "17"
   SELECTION_XML  ${CMAKE_SOURCE_DIR}/pycoek/cppyy/selection.xml
   INTERFACE_FILE ${CMAKE_SOURCE_DIR}/pycoek/cppyy/cppyy_interface.hpp
   HEADERS        ${LIB_HEADERS}
   INCLUDE_DIRS   ${LIB_INCLUDE_DIRS}
   LINK_LIBRARIES coek ${OPT_LINK_LIBRARIES}
   NAMESPACES     coek
   GENERATE_OPTIONS    -I${CMAKE_SOURCE_DIR}
)

add_custom_target(pip_install_cppyy
        DEPENDS pycoek_cppyy
        COMMAND pip install --force-reinstall ${PY_WHEEL_FILE}
        COMMAND python -c "import pycoek_cppyy"
        )
