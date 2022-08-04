
#find_package(Catch2 REQUIRED)
#add_library(Catch INTERFACE)
#target_include_directories(Catch INTERFACE ${CMAKE_INSTALL_PREFIX}/include)

find_package(fmt REQUIRED)
#add_library(fmt::fmt SHARED IMPORTED)

find_package(coek)
include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)
#include_directories(BEFORE ${COEK_INCLUDE_DIRS})
#LINK_DIRECTORIES(${COEK_LIBRARY_DIRS})
message("XHERE ${COEK_COMPILE_FLAGS}")

set(sources
    writer.cpp
    models/misc/pmedian.cpp
    models/misc/nqueens.cpp
    models/misc/knapsack.cpp
    models/jump/lqcp_scalar.cpp
    )
if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    list(APPEND sources
        models/jump/lqcp_array.cpp
        models/jump/lqcp_compact.cpp
        models/jump/lqcp_map.cpp
        )
endif()

# coek_writer
add_executable(coek_writer ${sources})
TARGET_LINK_LIBRARIES(coek_writer PRIVATE coek::coek fmt::fmt)
#TARGET_COMPILE_OPTIONS(coek_writer PUBLIC coek)
#TARGET_LINK_LIBRARIES(coek_writer PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
#TARGET_COMPILE_OPTIONS(coek_writer PUBLIC ${coek_compile_flags})

# rlqcp
##add_executable(rlqcp rlqcp.cpp)
##TARGET_LINK_LIBRARIES(rlqcp PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
##TARGET_COMPILE_OPTIONS(rlqcp PUBLIC ${coek_compile_flags})

# resolve
##add_executable(resolve resolve.cpp)
##TARGET_LINK_LIBRARIES(resolve PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
##TARGET_COMPILE_OPTIONS(resolve PUBLIC ${coek_compile_flags})

