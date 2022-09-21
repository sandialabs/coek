cmake_minimum_required(VERSION 3.7)

# Use pybind11 to create pycoek_pybind11 Python module

set(PYBIND11_CPP_STANDARD -std=c++17)

find_package(coek REQUIRED)
#find_package(fmt REQUIRED)
#include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

#LINK_DIRECTORIES(${coek_link_directories})
if(${with_debug})
    pybind11_add_module(pycoek_pybind11 NO_EXTRAS pycoek_pybind11.cpp)
else()
    pybind11_add_module(pycoek_pybind11 pycoek_pybind11.cpp)
endif()
TARGET_LINK_LIBRARIES(pycoek_pybind11 PUBLIC coek::coek)
#TARGET_COMPILE_OPTIONS(pycoek_pybind11 PUBLIC ${coek_compile_options})


install(TARGETS pycoek_pybind11
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
       )

#
# This is poorly named, but for consistency I'll stick with this for now
#
add_custom_target(pip_install_pybind11
        DEPENDS pycoek_pybind11
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/setup.py develop -u
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/setup.py develop
        )
