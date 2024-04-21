#!/bin/bash
#
# This scripts builds Coek in the `build` directory to support local
# development and debugging.
#
# This uses Spack to install third-party dependencies in the `spack` directory.
#
export SPACK_HOME=`pwd`/spack
echo "SPACK_HOME=${SPACK_HOME}"

if test -d ${SPACK_HOME}; then
    echo ""
    echo "WARNING: Spack directory exists."
    echo ""
else
    echo ""
    echo "Installing Coek dependencies using Spack"
    echo ""
    git clone git@github.com:or-fusion/spack.git
    . ${SPACK_HOME}/share/spack/setup-env.sh
    spack external find python
    spack external find gurobi
    spack env create coekenv
    spack env activate coekenv
    spack add asl cppad fmt rapidjson catch2
    spack install
    spack env deactivate
fi

echo "Building Coek"
echo ""
\rm -Rf build
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=${SPACK_HOME}/var/spack/environments/coekenv/.spack-env/view -Dwith_python=ON -Dwith_gurobi=ON -Dwith_cppad=ON -Dwith_fmtlib=ON -Dwith_rapidjson=ON -Dwith_catch2=ON -Dwith_tests=ON -Dwith_asl=ON -Dwith_openmp=OFF ..
make -j20
