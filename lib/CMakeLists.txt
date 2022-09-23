#
# CoekProject Libraries
#

message("")
message("Configuring lib/coek")
add_subdirectory(coek)

message("")
message("Configuring lib/pycoek")
add_subdirectory(pycoek)

# Add Poek installation target
add_custom_target(pip_install_poek ALL
        COMMAND ${PYTHON_EXECUTABLE} -m pip install -e ${CMAKE_CURRENT_SOURCE_DIR}/poek
        )

