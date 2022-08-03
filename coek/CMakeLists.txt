
find_package(Catch2 REQUIRED)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_INSTALL_PREFIX}/include)

find_package(coek REQUIRED)
find_package(fmt REQUIRED)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/..)
include_directories(BEFORE ${COEK_INCLUDE_DIRS})
LINK_DIRECTORIES(${COEK_LIBRARY_DIRS})


# WEH - what does this do?
enable_testing()


# lqcp
add_executable(lqcp lqcp.cpp)
TARGET_LINK_LIBRARIES(lqcp PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(lqcp PUBLIC ${coek_compile_flags})

# lqcp_array
if (CMAKE_CXX_STANDARD GREATER_EQUAL 17)
    add_executable(lqcp_array lqcp_array.cpp)
    TARGET_LINK_LIBRARIES(lqcp_array PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
    TARGET_COMPILE_OPTIONS(lqcp_array PUBLIC ${coek_compile_flags})
endif()

# lqcp_map
if (with_compact)
    add_executable(lqcp_map lqcp_map.cpp)
    TARGET_LINK_LIBRARIES(lqcp_map PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
    TARGET_COMPILE_OPTIONS(lqcp_map PUBLIC ${coek_compile_flags})
endif()

# lqcp2
if(with_compact)
    add_executable(lqcp2 lqcp2.cpp)
    TARGET_LINK_LIBRARIES(lqcp2 PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
    TARGET_COMPILE_OPTIONS(lqcp2 PUBLIC ${coek_compile_flags})
endif()

# rlqcp
add_executable(rlqcp rlqcp.cpp)
TARGET_LINK_LIBRARIES(rlqcp PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(rlqcp PUBLIC ${coek_compile_flags})

# resolve
add_executable(resolve resolve.cpp)
TARGET_LINK_LIBRARIES(resolve PUBLIC ${COEK_LIBRARY} ${coek_link_libraries})
TARGET_COMPILE_OPTIONS(resolve PUBLIC ${coek_compile_flags})

