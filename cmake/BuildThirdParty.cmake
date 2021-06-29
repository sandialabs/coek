include(ExternalProject)
include(CMakeParseArguments)

#------------------------------------------------------------------------------
# Setup the download_command variable if the insecure_downloads option is
# specified.
#------------------------------------------------------------------------------
function(define_download_command)
    list(GET ARGN "1" name)
    list(FIND ARGN "URL" _url)
    if(${_url} EQUAL -1)
        set(url "missing")
    else()
        math(EXPR _tmp "${_url}+1")
        list(GET ARGN ${_tmp} url)
    endif()
    list(FIND ARGN "SRC" _src)
    if(${_src} EQUAL -1)
        set(src "missing")
    else()
        math(EXPR _tmp "${_src}+1")
        list(GET ARGN ${_tmp} src)
    endif()

    if (WIN32)
        set(download_command "" PARENT_SCOPE)
        set(source_dir "downloads/${name}" PARENT_SCOPE)
    elseif(insecure_downloads)
        get_filename_component(_filename ${url} NAME)
        set(download_command "curl;-L;--insecure;${url};-o;${name}/${_filename};&&;tar;xzf;${name}/${_filename};--directory;${name}" PARENT_SCOPE)
        set(source_dir "downloads/${name}/${src}" PARENT_SCOPE)
    else()
        set(download_command "" PARENT_SCOPE)
        set(source_dir "downloads/${name}" PARENT_SCOPE)
    endif()
endfunction()

#------------------------------------------------------------------------------
# Macro used to register versions for third party packages. 
#------------------------------------------------------------------------------
macro(add_revision name)
    set(${name}_revision "${ARGN}")
endmacro()

#------------------------------------------------------------------------------
# Setup builds for third party packages
#------------------------------------------------------------------------------
macro(setup_builds)
    set(tpls)

    # Catch
    set(catch2_available OFF CACHE BOOL "Catch2 is available")
    if (build_catch2)
        define_download_command("NAME;catch2;${catch2_revision}")
        list(POP_FRONT catch2_revision _d1 _d2)     # Remove SRC
        ExternalProject_Add(catch2
            ${catch2_revision}  # specified in thirdparth.cmake
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/catch2
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            SOURCE_DIR          ${source_dir}
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS
                -DCATCH_BUILD_TESTING=OFF
                -DCATCH_ENABLE_WERROR=OFF
                -DCATCH_INSTALL_DOCS=OFF
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
            BUILD_ALWAYS FALSE
            EXCLUDE_FROM_ALL TRUE
            )
        set(catch2_available ON)
        list(APPEND tpls catch2)
    endif()

    # CPPAD
    set(cppad_available OFF CACHE BOOL "CppAD is available")
    if (build_cppad)
        define_download_command("NAME;cppad;${cppad_revision}")
        list(POP_FRONT cppad_revision _d1 _d2)      # Remove SRC
        ExternalProject_Add(cppad
            ${cppad_revision}   # specified in thirdparth.cmake
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/cppad
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            SOURCE_DIR          ${source_dir}
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -Dcppad_prefix=${CMAKE_INSTALL_PREFIX}
            BUILD_ALWAYS FALSE
            EXCLUDE_FROM_ALL TRUE
            )
        set(cppad_available ON)
        list(APPEND tpls cppad)
    endif()

    # fmtlib
    set(fmtlib_available OFF CACHE BOOL "FMT is available")
    if (build_fmtlib)
        define_download_command("NAME;fmtlib;${fmtlib_revision}")
        list(POP_FRONT fmtlib_revision _d1 _d2)      # Remove SRC
        ExternalProject_Add(fmtlib
            ${fmtlib_revision}   # specified in thirdparth.cmake
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/fmtlib
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            SOURCE_DIR          ${source_dir}
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DFMT_MASTER_PROJECT=OFF
                -DFMT_INSTALL=ON
            BUILD_ALWAYS FALSE
            EXCLUDE_FROM_ALL TRUE
            )
        set(fmtlib_available ON)
        list(APPEND tpls fmtlib)
    endif()

    # Pybind11
    set(pybind11_available OFF CACHE BOOL "Pybind11 is available")
    if (build_pybind11)
        define_download_command("NAME;pybind11;${pybind11_revision}")
        list(POP_FRONT pybind11_revision _d1 _d2)       # Remove SRC
        ExternalProject_Add(pybind11
            ${pybind11_revision}   # specified in thirdparth.cmake
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/pybind11
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            SOURCE_DIR          ${source_dir}
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS
                -DPYBIND11_MASTER_PROJECT=OFF
                -DPYBIND11_INSTALL=ON
                -DPYBIND11_TEST=OFF
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
            BUILD_ALWAYS FALSE
            EXCLUDE_FROM_ALL TRUE
            )
        set(pybind11_available ON)
        list(APPEND tpls pybind11)
    endif()

    # RapidJson
    set(rapidjson_available OFF CACHE BOOL "RapidJSON is available")
    if (build_rapidjson)
        define_download_command("NAME;rapidjson;${rapidjson_revision}")
        list(POP_FRONT rapidjson_revision _d1 _d2)      # Remove SRC
        ExternalProject_Add(rapidjson
            ${rapidjson_revision}   # specified in thirdparth.cmake
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/rapidjson
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            SOURCE_DIR          ${source_dir}
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DRAPIDJSON_BUILD_DOC=OFF
                -DRAPIDJSON_BUILD_EXAMPLES=OFF
                -DRAPIDJSON_BUILD_TEST=OFF
            BUILD_ALWAYS FALSE
            EXCLUDE_FROM_ALL TRUE
            )
        set(rapidjson_available ON)
        list(APPEND tpls rapidjson)
    endif()

    MESSAGE("-- Third Party Package Dependencies")
    MESSAGE("   Catch2 Library:    ${catch2_available}")
    MESSAGE("   CppAD Library:     ${cppad_available}")
    MESSAGE("   FMT Library:       ${fmtlib_available}")
    MESSAGE("   Pybind11 Library:  ${pybind11_available}")
    MESSAGE("   RapidJSON Library: ${rapidjson_available}")

    add_custom_target(tpls 
        DEPENDS ${tpls}
        COMMAND ${CMAKE_COMMAND} -Duse_superbuild=OFF ..
        )
endmacro()
