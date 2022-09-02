#
# rapidjson
#
set(rapidjson_available OFF PARENT_SCOPE)
#set(rapidjson_available OFF)
if (with_rapidjson)
    find_package(RapidJSON)

    #MESSAGE("FIND SUCCESS: ${RapidJSON_FOUND}")
    #MESSAGE("FIND CONSIDERED: ${RapidJSON_CONSIDERED_CONFIGS}")
    if (RapidJSON_FOUND)
        set(RAPIDJSON_INCLUDE_DIR ${RAPIDJSON_INCLUDE_DIRS} PARENT_SCOPE)
        set(rapidjson_available ON PARENT_SCOPE)
        #set(rapidjson_available ON)
    else()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson)
            set(RAPIDJSON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include PARENT_SCOPE)
            set(rapidjson_available ON PARENT_SCOPE)
            #set(rapidjson_available ON)
        endif()
    endif()
endif()

#
# Catch
#
set(catch2_available OFF PARENT_SCOPE)
if (with_tests)
    find_package(Catch2)

    #MESSAGE("FIND SUCCESS: ${Catch2_FOUND}")
    #MESSAGE("FIND CONSIDERED: ${Catch2_CONSIDERED_CONFIGS}")
    if (Catch2_FOUND)
        get_target_property(_CATCH2_INCLUDE_DIR Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
        set(CATCH2_INCLUDE_DIR ${_CATCH2_INCLUDE_DIR} PARENT_SCOPE)
        set(catch2_available ON PARENT_SCOPE)
        #set(rapidjson_available ON)
    else()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/catch)
            set(CATCH2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/catch/include PARENT_SCOPE)
            set(catch2_available ON PARENT_SCOPE)
            #set(catch_available ON)
        endif()
    endif()
endif()

#
# CppAD
#
set(cppad_available OFF PARENT_SCOPE)
if (with_cppad)
    find_package(cppad)
    #MESSAGE("FIND SUCCESS: ${cppad_FOUND}")
    #MESSAGE("FIND CONSIDERED: ${cppad_CONSIDERED_CONFIGS}")

    if (cppad_FOUND)
        set(CPPAD_INCLUDE_DIR ${CPPAD_INCLUDE_DIRS} PARENT_SCOPE)
        set(cppad_available ON PARENT_SCOPE)
        #set(cppad_available ON)
    else()
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/CppAD)
          MESSAGE("-- With CppAD Library: on")
          set(cppad_available ON PARENT_SCOPE)
          set(CPPAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CppAD/include PARENT_SCOPE)
        else()
          MESSAGE("-- With CppAD Library: off")
          set(cppad_available OFF PARENT_SCOPE)
        endif()
    endif()
endif()

#
# Caliper
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

#
# fmtlib
#
if (with_fmtlib)
    add_subdirectory(fmtlib)
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

#
# Trilinos (THIS IS BROKEN)
#
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Trilinos)
  set(with_sacado ON PARENT_SCOPE)
  MESSAGE("-- With Sacado Library: on")
else()
  MESSAGE("-- With Sacado Library: off")
endif()

