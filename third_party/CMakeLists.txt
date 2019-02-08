#
# Test whether a third-party package is a subdirectory.
# If so, then configure Coek to build with CMake.
#
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
  MESSAGE("-- With CppAD Library: available")
  set(cppad_available ON PARENT_SCOPE)
  set(CPPAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CppAD PARENT_SCOPE)
else()
  set(cppad_available OFF PARENT_SCOPE)
  MESSAGE("-- With CppAD Library: missing")
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Trilinos)
  set(with_sacado ON PARENT_SCOPE)
  MESSAGE("-- With Sacado Library: on")
else()
  MESSAGE("-- With Sacado Library: off")
endif()

