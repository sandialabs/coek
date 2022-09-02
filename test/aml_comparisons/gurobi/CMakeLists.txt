
find_package(GUROBI)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(sources
    models/misc/pmedian.cpp
    models/misc/nqueens.cpp
    models/misc/knapsack.cpp
    models/jump/lqcp.cpp
    models/jump/facility.cpp
    )
#if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
#    list(APPEND sources
#        models/jump/lqcp_array.cpp
#        models/jump/lqcp_compact.cpp
#        models/jump/lqcp_map.cpp
#        )
#endif()

##
## NOTE: These *.o files get generated twice.  How to not do that?
##

message("libs ${GUROBI_LIBRARY}")
message("includes ${GUROBI_INCLUDE_DIRS}")

# gurobi_writer
add_executable(gurobi_writer writer.cpp ${sources})
TARGET_LINK_LIBRARIES(gurobi_writer PRIVATE ${GUROBI_LIBRARY} ${GUROBI_CXX_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(gurobi_writer PUBLIC ${GUROBI_INCLUDE_DIRS})

# gurobi_solver0
add_executable(gurobi_solve0 solve0.cpp ${sources})
TARGET_LINK_LIBRARIES(gurobi_solve0 PRIVATE ${GUROBI_LIBRARY} ${GUROBI_CXX_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(gurobi_solve0 PUBLIC ${GUROBI_INCLUDE_DIRS})

