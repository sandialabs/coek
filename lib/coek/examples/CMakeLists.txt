#
# Test examples.
#
if (with_fmtlib)
    find_package(fmt REQUIRED)
endif()

#OPTION(STATIC_LINK "STATIC_LINK" OFF)

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
if (build_shared_libs)
    add_library(coekexamples SHARED ${sources})
    message("-- Building shared libcoekexamples library")
    set_target_properties(coekexamples PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
else()
    add_library(coekexamples STATIC ${sources})
    message("-- Building static libcoekexamples library")
endif()

target_compile_options(coekexamples PUBLIC ${coek_compile_options})
target_link_libraries(coekexamples PUBLIC coek)
