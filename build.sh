#!/bin/bash -e
#
# This scripts builds Coek in the `build` directory to support local
# development and debugging.
#
# This uses Spack to install third-party dependencies in the `spack` directory.
#
with_python="OFF"
spack_reinstall=0
spack_dev=0
clang=0
for arg ; do
    case "$arg" in
        --help)
                    echo "build_dev.sh [--python] [--clang] [--spack-reinstall] [--spack-dev] [--help]"
                    exit 
        ;;
        --python)
                    with_python="ON"
        ;;
        --clang)
                    clang=1
        ;;
        --spack-reinstall)
                    spack_reinstall=1
        ;;
        --spack-dev)
                    spack_dev=1
        ;;
        *)
                    echo "unknown option: ${arg}"
                    exit 
        ;;
    esac
done

#
# Setup directories
#
export SPACK_HOME=`pwd`/_spack
echo "SPACK_HOME=${SPACK_HOME}"
if [[ "${spack_reinstall}" -eq 1 ]]; then
    rm -Rf ${SPACK_HOME}
fi
\rm -Rf _build
#
# Configure gurobi
#
if [[ -z "${GUROBI_HOME}" ]]; then
    with_gurobi="OFF"
else
    with_gurobi="ON"
fi
#
# Configure clang
#
if [[ $clang -eq 1 ]]; then
   export CXX=clang++
   export CC=clang
fi
#
# Install spack
#
if test -d ${SPACK_HOME}; then
    echo ""
    echo "WARNING: Spack directory exists."
    echo ""
else
    echo ""
    echo "Installing Coek dependencies using Spack"
    echo ""
    if [[ "$spack_dev" -eq 0 ]]; then
        git clone https://github.com/or-fusion/spack.git _spack
    else
        git clone git@github.com:or-fusion/spack.git _spack
    fi
    . ${SPACK_HOME}/share/spack/setup-env.sh
    spack env create coekenv
    spack env activate coekenv
    spack add asl cppad fmt rapidjson catch2 highs
    spack install
    spack env deactivate
fi
#
# Install coek
#
echo "Building Coek"
echo ""
mkdir _build
cd _build
cmake -DCMAKE_PREFIX_PATH=${SPACK_HOME}/var/spack/environments/coekenv/.spack-env/view -Dwith_python=${with_python} -Dwith_gurobi=$with_gurobi -Dwith_highs=ON -Dwith_cppad=ON -Dwith_fmtlib=ON -Dwith_rapidjson=ON -Dwith_catch2=ON -Dwith_tests=ON -Dwith_asl=ON -Dwith_openmp=OFF ..
make -j20
