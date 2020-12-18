#!/bin/sh
#

./getFmtlib.sh
cd fmtlib
mkdir build
cd build
cmake -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DFMT_TEST=OFF -DCMAKE_INSTALL_PREFIX=`pwd`/../install ..
make && make install
cd ../..

