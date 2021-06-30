#!/bin/sh

\rm -Rf build
mkdir build
cd build
cmake -Dbuild_all=ON -Dwith_ipopt=ON -Dwith_tests=ON -Dwith_pybind11=ON ..
make -j4 tpls
make -j10
make install
