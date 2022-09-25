cmake_minimum_required(VERSION 3.12)
PROJECT(pycoek)

#
# For now, let's just assume we have C++17
#
set(CMAKE_CXX_STANDARD 17)

option(conda_install "If enabled, then install in the current conda environment" OFF)

if( (conda_install) AND (DEFINED ENV{CONDA_PREFIX}) AND (with_pybind11 OR with_cppyy) )
    message(STATUS "Installing in a conda environment: $ENV{CONDA_PREFIX}")
    set(CONDA_ACTIVE TRUE)
    set(CMAKE_INSTALL_PREFIX "$ENV{CONDA_PREFIX}")
    list(APPEND CMAKE_PREFIX_PATH "$ENV{CONDA_PREFIX}")
    set(CMAKE_INCLUDE_PATH "$ENV{CONDA_PREFIX}/include")
    set(CMAKE_LIBRARY_PATH "$ENV{CONDA_PREFIX}/lib")
endif()


##################### SubDirectories #####################

# pybind11
MESSAGE("-- with pybind11: ${with_pybind11}")
if(with_pybind11)
    if (install_pybind11)
        set(CMAKE_MODULE_PATH ${download_dir}/pybind11/tools;${CMAKE_MODULE_PATH})
    endif()
    find_package(pybind11)
    if (${pybind11_FOUND})
        add_subdirectory(pybind11)
    endif()

    #
    # Note that this target is automatically build when with_pybind11 is ON
    #
    add_custom_target(python_pybind11
        DEPENDS pycoek_pybind11)
    add_custom_target(test_python_pybind11
        DEPENDS pycoek_pybind11
        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/pybind11 ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
endif()

# cppyy
MESSAGE("-- with cppyy: ${with_cppyy}")
if(with_cppyy)
    add_subdirectory(cppyy)

    #
    # Note that this target is automatically build when with_cppyy is ON
    #
    add_custom_target(python_cppyy
        DEPENDS pycoek_cppyy)
    add_custom_target(test_python_cppyy
        DEPENDS pycoek_cppyy
        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/cppyy ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )
endif()

# cffi
#if(with_cffi)
#    MESSAGE("-- With Cffi: on")
#    add_subdirectory(cffi)
#
#    #
#    # Note that this target is automatically build when with_cffi is ON
#    #
#    add_custom_target(python_cffi
#        DEPENDS pycoek)
#    add_custom_target(test_python_cffi
#        DEPENDS pycoek
#        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/cffi ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#        )
#else()
#    MESSAGE("-- With Cffi: off")
#endif()

