
#find_package(fmt REQUIRED)

#find_package(coek)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(sources
    models/jump/lqcp_scalar.cpp
    models/jump/fac_scalar.cpp
    models/misc/pmedian.cpp
    models/misc/nqueens.cpp
    models/misc/knapsack.cpp
    )
if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    list(APPEND sources
        models/jump/fac_array.cpp
        models/jump/lqcp_array.cpp
        models/misc/pmedian_array.cpp
        models/misc/nqueens_array.cpp
        models/misc/knapsack_array.cpp
        models/jump/lqcp_compact.cpp
        models/jump/lqcp_map.cpp
        )
endif()

##
## NOTE: These *.o files get generated twice.  How to not do that?
##

# coek_writer
add_executable(coek_writer writer.cpp ${sources})
TARGET_LINK_LIBRARIES(coek_writer PRIVATE coek::coek)
if (${with_gprof})
    target_compile_options(coek_writer PUBLIC "-pg")
    SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -pg")
endif()

# coek_solver0
add_executable(coek_solve0 solve0.cpp ${sources})
TARGET_LINK_LIBRARIES(coek_solve0 PRIVATE coek::coek)

# coek_micro
add_executable(coek_micro micro.cpp)
TARGET_LINK_LIBRARIES(coek_micro PRIVATE coek::coek)

# rlqcp
##add_executable(rlqcp rlqcp.cpp)
##TARGET_LINK_LIBRARIES(rlqcp PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
##TARGET_COMPILE_OPTIONS(rlqcp PUBLIC ${coek_compile_flags})

# resolve
##add_executable(resolve resolve.cpp)
##TARGET_LINK_LIBRARIES(resolve PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
##TARGET_COMPILE_OPTIONS(resolve PUBLIC ${coek_compile_flags})

