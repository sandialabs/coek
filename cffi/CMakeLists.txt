cmake_minimum_required(VERSION 3.7)

# Use cffi to create pycoek Python module

include_directories("${PROJECT_SOURCE_DIR}/src/")
include_directories("${PROJECT_SOURCE_DIR}/src/expr/")

#
# This is poorly named, but for consistency I'll stick with this for now
#
add_custom_target(pip_install
        DEPENDS pycoek
        COMMAND python ${PROJECT_SOURCE_DIR}/pycoek/cffi/setup.py develop
        )
