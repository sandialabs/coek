#
# CoekProject Libraries
#

message("")
message("Configuring lib/coek build")
add_subdirectory(coek)
set(lib_test_targets "test_coek")

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
    # Wrapper for poek testing
    #
    if(${with_tests})
        list(APPEND lib_test_targets "test_poek")
        add_custom_target(test_poek
            COMMAND ${Python_EXECUTABLE} -m pytest ${CMAKE_CURRENT_SOURCE_DIR}/poek/poek
            )
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
else()
    message("Ignoring lib/pyomo_coek")
endif()

if(${with_tests})
  add_custom_target(test_lib DEPENDS ${lib_test_targets})
endif()
