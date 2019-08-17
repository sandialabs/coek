#!/bin/sh -v
#
# Enable debugging by executing: 
#    bin/trace.sh forward0 1
#    bin/trace.sh forward1 1
#

./getCppAD.sh
cd CppAD
mkdir build
cd build
cmake -Dcppad_prefix=`pwd`/.. ..
make install
cd ../..

