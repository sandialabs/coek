#
# Test examples.
#
if (fmtlib_available)
    find_package(fmt REQUIRED)
endif()

OPTION(STATIC_LINK "STATIC_LINK" OFF)

SET(sources
    simplelp1.cpp
    simplelp1_solve.cpp
    rosenbr.cpp
    srosenbr_vector.cpp
    srosenbr_array.cpp
    invquad_vector.cpp
    invquad_array.cpp
    invquad_array_solve.cpp
    invquad_array_resolve.cpp
   )

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)

# build library
ADD_LIBRARY(coekexamples ${sources})
#TARGET_COMPILE_OPTIONS(coekexamples PRIVATE ${coek_compile_options})
target_compile_options(coekexamples PUBLIC ${coek_compile_options})
target_link_libraries(coekexamples PUBLIC ${coek_link_libraries})

