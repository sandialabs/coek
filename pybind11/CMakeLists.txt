cmake_minimum_required(VERSION 3.7)

# Use pybind11 to create pycoek Python module

set(PYBIND11_CPP_STANDARD -std=c++17)

include_directories("${PROJECT_SOURCE_DIR}/src/")
include_directories("${PROJECT_SOURCE_DIR}/src/expr/")

if(${with_gurobi})
  LINK_DIRECTORIES($ENV{GUROBI_HOME}/lib)
endif()
if(${with_ipopt})
  LINK_DIRECTORIES($ENV{IPOPT_HOME}/lib)
endif()

if(${with_debug})
    pybind11_add_module(pycoek NO_EXTRAS pycoek.cpp)
else()
    pybind11_add_module(pycoek pycoek.cpp)
endif()
TARGET_LINK_LIBRARIES(pycoek PUBLIC coek)
if(${with_gurobi})
  TARGET_LINK_LIBRARIES(pycoek PUBLIC gurobi_g++5.2 gurobi90)
endif()
if(${with_ipopt})
  TARGET_LINK_LIBRARIES(pycoek PUBLIC ipopt gfortran)
endif()


INSTALL(TARGETS pycoek
        DESTINATION lib)

#
# This is poorly named, but for consistency I'll stick with this for now
#
add_custom_target(pip_install
        DEPENDS pycoek
        COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pybind11/setup.py develop -u
        COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pybind11/setup.py develop
        )
