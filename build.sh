#!/bin/bash -e
#
# This scripts builds Coek in the `_build` directory to support local
# development and debugging.
#
# This uses Spack to install third-party dependencies in the `_spack` directory.
#
with_python="OFF"
python_exe=""
spack_reinstall=0
spack_dev=0
clang=0
debug="OFF"
spack_home=`pwd`/_spack
with_valgrind=""
while [[ $# -gt 0 ]]; do
    case $1 in
        --help)
                    echo "build.sh [--help] [--clang] [--debug] [--python] [--python-exe <file>] [--spack-dev] [--spack-home <dir>] [--spack-reinstall] [--valgrind]"
                    exit 
        ;;
        --python)
                    with_python="ON"
                    shift
        ;;
        --python-exe)
                    python_exe="-DPython_EXECUTABLE=$2"
                    shift
                    shift
        ;;
        --clang)
                    clang=1
                    shift
        ;;
        --debug)
                    debug="ON"
                    shift
        ;;
        --valgrind)
                    with_valgrind="valgrind"
                    shift
        ;;
        --spack-reinstall)
                    spack_reinstall=1
                    shift
        ;;
        --spack-dev)
                    spack_dev=1
                    shift
        ;;
        --spack-home)
                    spack_home="$2"
                    shift
                    shift
        ;;
        *)
                    echo "unknown option: $1"
                    exit 
        ;;
    esac
done

#
# Setup directories
#
export SPACK_HOME=${spack_home}
echo "SPACK_HOME=${SPACK_HOME}"
if [[ "${spack_reinstall}" -eq 1 ]]; then
    \rm -Rf ${SPACK_HOME}
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
        git clone https://github.com/or-fusion/spack.git ${SPACK_HOME}
    else
        git clone git@github.com:or-fusion/spack.git ${SPACK_HOME}
    fi
    . ${SPACK_HOME}/share/spack/setup-env.sh
    spack env create coekenv
    spack env activate coekenv
    spack add asl cppad fmt rapidjson catch2 highs $with_valgrind
    spack install
    spack env deactivate
fi
if test -d ${SPACK_HOME}; then
    export SPACK_HOME=$(cd ${SPACK_HOME}; pwd)
    echo "SPACK_HOME=${SPACK_HOME}"
fi
#
# Install coek
#
echo "Building Coek"
echo ""
mkdir _build
cd _build
cmake -DCMAKE_PREFIX_PATH=${SPACK_HOME}/var/spack/environments/coekenv/.spack-env/view -Dwith_debug=${debug} -Dwith_python=${with_python} $python_exe -Dwith_gurobi=$with_gurobi -Dwith_highs=ON -Dwith_cppad=ON -Dwith_fmtlib=ON -Dwith_rapidjson=ON -Dwith_catch2=ON -Dwith_tests=ON -Dwith_asl=ON -Dwith_openmp=OFF ..
make -j20
