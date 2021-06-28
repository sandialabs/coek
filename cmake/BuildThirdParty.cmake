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
    if (WIN32)
        set(download_command "" PARENT_SCOPE)
    elseif(insecure_downloads)
        get_filename_component(_filename ${url} NAME)
        #set(download_command "curl --insecure ${url} -o ${_filename}" PARENT_SCOPE)
        set(download_command "curl;-L;--insecure;${url};-o;${name}/${_filename};&&;tar;xzf;${name}/${_filename};--directory;${name}" PARENT_SCOPE)
    else()
        set(download_command "" PARENT_SCOPE)
    endif()
    #message("D ${download_command}")
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
    # Catch
    set(catch2_available OFF)
    if (build_catch2)
        define_download_command("NAME;catch2;${catch2_revision}")
        ExternalProject_Add(catch2
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/catch2
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            #SOURCE_DIR          ${download_dir}/catch2
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            ${catch2_revision}  # specified in thirdparth.cmake
            CMAKE_ARGS
                -DCATCH_BUILD_TESTING=OFF
                -DCATCH_ENABLE_WERROR=OFF
                -DCATCH_INSTALL_DOCS=OFF
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
            BUILD_ALWAYS TRUE
            )
        set(catch2_available ON)
    endif()

    # CPPAD
    set(cppad_available OFF)
    if (build_cppad)
        define_download_command("NAME;cppad;${cppad_revision}")
        ExternalProject_Add(cppad
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/cppad
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            #SOURCE_DIR          ${download_dir}/cppad
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            ${cppad_revision}   # specified in thirdparth.cmake
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -Dcppad_prefix=${CMAKE_INSTALL_PREFIX}
            BUILD_ALWAYS TRUE
            )
        set(cppad_available ON)
    endif()

    # RapidJson
    set(rapidjson_available OFF)
    if (build_rapidjson)
        define_download_command("NAME;rapidjson;${rapidjson_revision}")
        ExternalProject_Add(rapidjson
            PREFIX              ${CMAKE_CURRENT_BINARY_DIR}/build/rapidjson
            DOWNLOAD_DIR        ${download_dir}
            DOWNLOAD_COMMAND    ${download_command}
            #SOURCE_DIR          ${download_dir}/rapidjson
            INSTALL_DIR         ${CMAKE_INSTALL_PREFIX}
            ${rapidjson_revision}   # specified in thirdparth.cmake
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                -DRAPIDJSON_BUILD_DOC=OFF
                -DRAPIDJSON_BUILD_EXAMPLES=OFF
                -DRAPIDJSON_BUILD_TEST=OFF
            BUILD_ALWAYS TRUE
            )
        set(rapidjson_available ON)
    endif()

    MESSAGE("-- Third Party Package Dependencies")
    MESSAGE("   Catch2 Library:    ${catch2_available}")
    MESSAGE("   CppAD Library:     ${cppad_available}")
    MESSAGE("   RapidJSON Library: ${rapidjson_available}")
endmacro()
