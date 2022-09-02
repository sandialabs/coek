#!/bin/sh
#

./getCaliper.sh
cd Caliper
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=`pwd`/../install ..
make && make install
cd ../..

