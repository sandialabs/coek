#
# CoekProject Libraries
#

message("")
message("Configuring lib/coek build")
add_subdirectory(coek)

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
    add_custom_target(pip_install_poek ALL
        COMMAND ${Python_EXECUTABLE} -m pip install -e ${CMAKE_CURRENT_SOURCE_DIR}/poek
        )
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
