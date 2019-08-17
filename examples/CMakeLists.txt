#
# Test examples.
#

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    simplelp1.cpp
    rosenbr.cpp
    srosenbr.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR})

# build library
ADD_LIBRARY(coekexamples ${sources})
TARGET_COMPILE_OPTIONS(coekexamples PRIVATE -I${CMAKE_CURRENT_SOURCE_DIR}/../src)

