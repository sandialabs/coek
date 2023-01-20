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

    # ASL
    set(asl_available OFF CACHE BOOL "ASL is available")
    if (install_asl)
        ExternalProject_Add(libasl
            #${asl_revision}   # specified in thirdparty.cmake
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/ASL
            DOWNLOAD_DIR        ${download_dir}
            GIT_REPOSITORY      "https://github.com/whart222/asl.git"
            SOURCE_DIR          "downloads/ASL"
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DBUILD_SHARED_LIBS=ON
                -DCMAKE_BUILD_TYPE=Release
            BUILD_ALWAYS FALSE
            EXCLUDE_FROM_ALL TRUE
            )
        set(asl_available ON)
        list(APPEND tpls libasl)
    endif()

    # Catch
    set(catch2_available OFF CACHE BOOL "Catch2 is available")
    if (install_catch2)
        define_download_command("NAME;catch2;${catch2_revision}")
        list(REMOVE_AT catch2_revision 0 1)         # Remove SRC
        ExternalProject_Add(catch2
            ${catch2_revision}  # specified in thirdparty.cmake
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
    if (install_cppad)
        define_download_command("NAME;cppad;${cppad_revision}")
        list(REMOVE_AT cppad_revision 0 1)         # Remove SRC
        ExternalProject_Add(cppad
            ${cppad_revision}   # specified in thirdparty.cmake
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
    if (install_fmtlib)
        define_download_command("NAME;fmtlib;${fmtlib_revision}")
        list(REMOVE_AT fmtlib_revision 0 1)         # Remove SRC
        ExternalProject_Add(fmtlib
            ${fmtlib_revision}   # specified in thirdparty.cmake
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/fmtlib
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            SOURCE_DIR          ${source_dir}
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DFMT_MASTER_PROJECT=OFF
                -DFMT_INSTALL=ON
                -DBUILD_SHARED_LIBS=ON
            BUILD_ALWAYS FALSE
            EXCLUDE_FROM_ALL TRUE
            )
        set(fmtlib_available ON)
        list(APPEND tpls fmtlib)
    endif()

    # Pybind11
    set(pybind11_available OFF CACHE BOOL "Pybind11 is available")
    if (install_pybind11)
        define_download_command("NAME;pybind11;${pybind11_revision}")
        list(REMOVE_AT pybind11_revision 0 1)         # Remove SRC
        ExternalProject_Add(pybind11
            ${pybind11_revision}   # specified in thirdparty.cmake
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
    if (install_rapidjson)
        define_download_command("NAME;rapidjson;${rapidjson_revision}")
        list(REMOVE_AT rapidjson_revision 0 1)         # Remove SRC
        ExternalProject_Add(rapidjson
            ${rapidjson_revision}   # specified in thirdparty.cmake
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

    MESSAGE("Configured to Install Third Party Packages (use 'make install_tpls')")
    MESSAGE("   ASL Library:       ${asl_available}")
    MESSAGE("   Catch2 Library:    ${catch2_available}")
    MESSAGE("   CppAD Library:     ${cppad_available}")
    MESSAGE("   FMT Library:       ${fmtlib_available}")
    MESSAGE("   Pybind11 Library:  ${pybind11_available}")
    MESSAGE("   RapidJSON Library: ${rapidjson_available}")

    #
    # Enable pybind11 here, since we now have the pybind11 cmake file
    #
    add_custom_target(_install_tpls
        DEPENDS ${tpls}
        COMMAND ${CMAKE_COMMAND} -Duse_superbuild=OFF ${PROJECT_SOURCE_DIR}
        #COMMAND ${CMAKE_COMMAND} rebuild_cache
        #COMMAND ${CMAKE_COMMAND} -Dwith_pybind11=ON ${PROJECT_SOURCE_DIR}/..
        )
endmacro()
