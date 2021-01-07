#
# Add fmtlib
#
if (with_fmtlib)
    add_subdirectory(fmtlib)
endif()

#
# Test whether a third-party package is a subdirectory.
# If so, then configure Coek to build with CMake.
#
if(with_caliper AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Caliper)
  MESSAGE("-- With Caliper Library: on")
  #set(with_caliper ON PARENT_SCOPE)
  set(CALIPER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Caliper/install/include PARENT_SCOPE)
  set(CALIPER_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Caliper/install/lib64 PARENT_SCOPE)
else()
  MESSAGE("-- With Caliper Library: off")
  set(with_caliper OFF PARENT_SCOPE)
endif()

if(with_fmtlib AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/fmtlib)
  MESSAGE("-- With fmtlib Library: on")
  #set(with_fmtlib ON PARENT_SCOPE)
  set(FMTLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fmtlib/include PARENT_SCOPE)
  #set(FMTLIB_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/fmtlib/lib64 PARENT_SCOPE)
else()
  MESSAGE("-- With fmtlib Library: off")
  set(with_fmtlib OFF PARENT_SCOPE)
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
  MESSAGE("-- With CppAD Library: on")
  set(cppad_available ON PARENT_SCOPE)
  set(CPPAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CppAD/include PARENT_SCOPE)
else()
  set(cppad_available OFF PARENT_SCOPE)
  MESSAGE("-- With CppAD Library: off")
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Trilinos)
  set(with_sacado ON PARENT_SCOPE)
  MESSAGE("-- With Sacado Library: on")
else()
  MESSAGE("-- With Sacado Library: off")
endif()

