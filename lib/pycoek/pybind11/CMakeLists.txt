cmake_minimum_required(VERSION 3.7)

# Use pybind11 to create pycoek_pybind11 Python module

set(PYBIND11_CPP_STANDARD -std=c++17)

if(${with_debug})
    pybind11_add_module(pycoek_pybind11 NO_EXTRAS pycoek_pybind11.cpp)
else()
    pybind11_add_module(pycoek_pybind11 pycoek_pybind11.cpp)
endif()
TARGET_LINK_LIBRARIES(pycoek_pybind11 PUBLIC coek::coek)


# Install pycoek_pybind11 in install directory
install(TARGETS pycoek_pybind11
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
       )

# Uninstall pycoek_pybind11 in python environment
add_custom_target(uninstall_pycoek_pybind11
        COMMAND ${Python_EXECUTABLE} -m pip uninstall -y pycoek_pybind11
        )

# Install pycoek_pybind11 in python environment, after uninstalling it
add_custom_target(install_pycoek_pybind11 ALL
        DEPENDS uninstall_pycoek_pybind11
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/setup.py
                ${CMAKE_CURRENT_BINARY_DIR}/setup.py
        COMMAND ${Python_EXECUTABLE} -m pip install -e ${CMAKE_CURRENT_BINARY_DIR}
        )

