# This maintains the configuration for third-party packages that can be installed 
# with the Coek Project.  Simply update this file to change the revision.
# One can use different revision on different platforms.
# e.g.
# if (UNIX)
#   ..
# else (APPLE)
#   ..
# endif()

add_revision(catch2
  SRC Catch2-2.13.6
  URL "https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.6.tar.gz"
  URL_MD5 a01187eecc1af812f6a6cdec2e107c91
  )

add_revision(cppad
  SRC CppAD-20220000.5
  URL "https://github.com/coin-or/CppAD/archive/refs/tags/20220000.5.tar.gz"
  URL_MD5 0e9694fc6f8e037b0416d9a4d57e8ef2
  )

add_revision(rapidjson
  SRC rapidjson-1.1.0
  URL "https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.tar.gz"
  URL_MD5 119686608b0571e1fa5c7b03efb2526f
  )

add_revision(pybind11
  SRC "pybind11-2.6.2"
  URL "https://github.com/pybind/pybind11/archive/refs/tags/v2.6.2.tar.gz"
  URL_MD5 c5ea9c4c57082e05efe14e4b34323bfd
  )

add_revision(fmtlib
  SRC "fmt-8.0.0"
  URL "https://github.com/fmtlib/fmt/archive/refs/tags/8.0.0.tar.gz"
  URL_MD5 001d59c967115682aed0e836ba5753a8
  )

