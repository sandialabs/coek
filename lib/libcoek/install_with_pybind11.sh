#!/bin/sh
#
# NOTE: This script can pass-in cmake configuration options:
#
#     ./install_with_pybind11.sh -Dwith_compact=ON 
#

\rm -Rf build
mkdir build
cd build
cmake -Dbuild_all=ON $* -Dwith_tests=ON -Dwith_pybind11=ON ..
make -j4 tpls
make -j10
make install
