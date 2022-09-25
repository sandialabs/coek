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
