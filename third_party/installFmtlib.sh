#!/bin/sh
#

./getFmtlib.sh
cd fmtlib
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install ..
make && make install
cd ../..

