cmake_minimum_required(VERSION 3.7)

# Use pybind11 to create pycoek_pybind11 Python module

set(PYBIND11_CPP_STANDARD -std=c++17)

if (fmtlib_available)
    find_package(fmt REQUIRED)
endif()
include_directories("${PROJECT_SOURCE_DIR}")

if(${with_gurobi})
  LINK_DIRECTORIES($ENV{GUROBI_HOME}/lib)
endif()
if(${with_ipopt})
  LINK_DIRECTORIES($ENV{IPOPT_HOME}/lib)
endif()

LINK_DIRECTORIES(${coek_link_directories})
if(${with_debug})
    pybind11_add_module(pycoek_pybind11 NO_EXTRAS pycoek_pybind11.cpp)
else()
    pybind11_add_module(pycoek_pybind11 pycoek_pybind11.cpp)
endif()
TARGET_LINK_LIBRARIES(pycoek_pybind11 PUBLIC coek ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(pycoek_pybind11 PUBLIC ${coek_compile_options})


INSTALL(TARGETS pycoek_pybind11
        DESTINATION lib)

#
# This is poorly named, but for consistency I'll stick with this for now
#
add_custom_target(pip_install_pybind11
        DEPENDS pycoek_pybind11
        COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pycoek/pybind11/setup.py develop -u
        COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/pycoek/pybind11/setup.py develop
        )
