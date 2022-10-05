cmake_minimum_required(VERSION 3.12)
PROJECT(coek
        #LANGUAGES CXX
        #VERSION 1.0.0
        #DESCRIPTION "C++ Optimization Expression Kernels"
        )
if(${CMAKE_VERSION} VERSION_LESS "3.5.0")
    cmake_policy(SET CMP0022 NEW)
endif()

set(COEK_VERSION 1.0.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/lib/cmake;${CMAKE_MODULE_PATH})



##################### Coek Options #####################

# Configuration options
option(with_compact "Add support for compact expressions" OFF)
option(with_examples "Build examples" OFF)

# Autograd Dependencies
option(with_sacado "Use the Sacado autograd library" OFF)
option(with_asl "Use the ASL autograd library" OFF)

# Solver Dependencies
option(with_gurobi "Use the Gurobi solver" OFF)
set(GUROBI_HOME "" CACHE FILEPATH "Set the path to gurobi")


##################### Other Config #####################

# define build type
MESSAGE("-- With Compact Models: ${with_compact}")
if(with_compact)
    list(APPEND coek_compile_options "-DCOEK_WITH_COMPACT_MODEL")
endif()

##################### Solvers #####################

MESSAGE("-- With Gurobi Solver: ${with_gurobi}")
if(with_gurobi)
  find_package(GUROBI)
  list(APPEND solver_flags "-DWITH_GUROBI")
endif()


##################### SubDirectories #####################

# coek
add_subdirectory(coek)

# examples
if (with_examples OR with_tests)
    add_subdirectory(examples)
endif()

# testing
if (${with_tests} AND ${with_catch2})
  MESSAGE("-- With Tests: ON")
  add_subdirectory(test)

  add_custom_target(test_coek
    COMMAND ${CMAKE_CTEST_COMMAND})
  add_custom_target(test_coek_verbose
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
  add_custom_target(test_coek_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action memcheck
    COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log")

  if (gcovr_FOUND)
    add_custom_target(gcovr_coek
      COMMAND ${GCOVR_EXECUTABLE} -d -r ${CMAKE_CURRENT_SOURCE_DIR} -e ../third_party/ -e ../test/ -e install/ )
  endif()
else()
  MESSAGE("-- With Tests: OFF")
endif()

# docs
if (with_docs)
    add_subdirectory(doc)
endif()

# cffi
#if(with_cffi)
#    MESSAGE("-- With Cffi: on")
#    add_subdirectory(cffi)
#
#    #
#    # Note that this target is automatically build when with_cffi is ON
#    #
#    add_custom_target(python_cffi
#        DEPENDS pycoek)
#    add_custom_target(test_python_cffi
#        DEPENDS pycoek
#        COMMAND nosetests ${CMAKE_CURRENT_SOURCE_DIR}/cffi ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#        )
#else()
#    MESSAGE("-- With Cffi: off")
#endif()


# =============================================================================
# Provide config and version files to be used by other applications
# =============================================================================

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/coekConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/coekConfig.cmake
    INSTALL_DESTINATION ${LIBRARY_INSTALL_DIR}/cmake/coek
    PATH_VARS INCLUDE_INSTALL_DIR LIBRARY_INSTALL_DIR)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/coekConfigVersion.cmake
    VERSION ${COEK_VERSION}
    COMPATIBILITY AnyNewerVersion )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/coekConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/coekConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coek )

install(EXPORT coekTargets
        FILE coekTargets.cmake
        NAMESPACE coek::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/coek
)

