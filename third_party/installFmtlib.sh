#!/bin/sh
#

./getFmtlib.sh
exit 0
cd fmtlib
mkdir build
cd build
cmake -DMASTER_PROJECT=OFF -DCMAKE_INSTALL_PREFIX=`pwd`/../install ..
make && make install
cd ../..

