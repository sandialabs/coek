#
# Test whether a third-party package is a subdirectory.
# If so, then configure Coek to build with CMake.
#
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
  MESSAGE("-- With CppAD Library: on")
  set(with_cppad ON PARENT_SCOPE)
  set(CPPAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CppAD PARENT_SCOPE)
else()
  MESSAGE("-- With CppAD Library: off")
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Trilinos)
  set(with_sacado ON PARENT_SCOPE)
  MESSAGE("-- With Sacado Library: on")
else()
  MESSAGE("-- With Sacado Library: off")
endif()

