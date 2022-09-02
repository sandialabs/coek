#
# Test examples.
#
if (fmtlib_available)
    find_package(fmt REQUIRED)
endif()

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    simplelp1.cpp
    rosenbr.cpp
    srosenbr.cpp
    invquad.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# build library
ADD_LIBRARY(coekexamples ${sources})
#TARGET_COMPILE_OPTIONS(coekexamples PRIVATE ${coek_compile_options})
target_compile_options(coekexamples PUBLIC ${coek_compile_options})
target_link_libraries(coekexamples PUBLIC ${coek_link_libraries})

