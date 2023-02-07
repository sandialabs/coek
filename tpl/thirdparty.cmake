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
  URL_HASH SHA256=48dfbb77b9193653e4e72df9633d2e0383b9b625a47060759668480fdf24fbd4
  )

add_revision(cppad
  SRC CppAD-20220000.5
  URL "https://github.com/coin-or/CppAD/archive/refs/tags/20220000.5.tar.gz"
  URL_HASH SHA256=9fb4562f6169855eadcd86ac4671593d1c0edf97bb6ce7cbb28e19af2bfc165e
  )

add_revision(fmtlib
  SRC "fmt-8.0.0"
  URL "https://github.com/fmtlib/fmt/archive/refs/tags/8.0.0.tar.gz"
  URL_HASH SHA256=7bce0e9e022e586b178b150002e7c2339994e3c2bbe44027e9abb0d60f9cce83
  )

add_revision(pybind11
  SRC "pybind11-2.10.3"
  URL "https://github.com/pybind/pybind11/archive/refs/tags/v2.10.3.tar.gz"
  URL_HASH SHA256=5d8c4c5dda428d3a944ba3d2a5212cb988c2fae4670d58075a5a49075a6ca315
  )

add_revision(rapidjson
  SRC rapidjson-1.1.0
  URL "https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.tar.gz"
  URL_HASH SHA256=bf7ced29704a1e696fbccf2a2b4ea068e7774fa37f6d7dd4039d0787f8bed98e
  )

