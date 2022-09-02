#!/bin/sh
#
# Enable debugging by executing: 
#    bin/trace.sh forward0 1
#    bin/trace.sh forward1 1
#

./getCppAD.sh
cd CppAD
mkdir build
cd build
cmake -Dcppad_debug_which=debug_none -Dcppad_prefix=`pwd`/.. ..
make install
cd ../..

