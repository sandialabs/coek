project(coek_tpl)


include(download_dir)
include(BuildThirdParty)
include(thirdparty.cmake)

setup_builds()
if (build_dependencies)
    return()
endif()

if (with_catch2 AND (NOT build_catch2))
    find_package(Catch2)
    if (catch2_FOUND)
        set(catch2_available ON)
    else()
        set(catch2_available OFF)
    endif()
endif()

if (with_cppad AND (NOT build_cppad))
    find_package(cppad)
    if (cppad_FOUND)
        set(cppad_available ON)
    else()
        set(cppad_available OFF)
    endif()
endif()

if (with_rapidjson AND (NOT build_rapidjson))
    find_package(RapidJSON)
    if (RapidJSON_FOUND)
        set(rapidjson_available ON)
    else()
        set(rapidjson_available OFF)
    endif()
endif()

message("TPL SUMMARY")
message("-- Catch2 available:    ${catch2_available}")
message("-- CppAD available:     ${cppad_available}")
message("-- RapidJSON available: ${rapidjson_available}")

