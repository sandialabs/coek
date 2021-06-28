# This maintains the configuration for third-party packages that can be installed 
# with COEK.  Simply update this file to change the revision.
# One can use different revision on different platforms.
# e.g.
# if (UNIX)
#   ..
# else (APPLE)
#   ..
# endif()

add_revision(catch2
  URL "https://github.com/catchorg/Catch2/archive/refs/tags/v2.13.6.tar.gz"
  URL_MD5 c7c7ef181b9e08418fd9f2ef8159d03f
  SOURCE_DIR downloads/catch2/Catch2-2.13.6
  )

add_revision(cppad
  URL "https://github.com/coin-or/CppAD/archive/refs/tags/20210000.6.tar.gz"
  URL_MD5 d63b03bce0417c420e610cb1cfb64d33
  SOURCE_DIR downloads/cppad/CppAD-20210000.6
  )

add_revision(rapidjson
  URL "https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.tar.gz"
  URL_MD5 d41d8cd98f00b204e9800998ecf8427e
  SOURCE_DIR downloads/rapidjson/rapidjson-1.1.0
  )

