#!/bin/bash -ev
#
# This scripts builds Coek in the `_build` directory to support local
# development and debugging.
#
# This uses Spack to install third-party dependencies in the `_spack` directory.
#
clang=0
debug="OFF"
python_exe=""
spack_dev=0
spack_env="coekenv"
spack_home=`pwd`/_spack
spack_reinstall=0
with_python="OFF"
with_valgrind=""
while [[ $# -gt 0 ]]; do
    case $1 in
        --help)
                    echo "build.sh [--help] [--clang] [--debug] [--python] [--python-exe <file>] [--spack-dev] [--spack-env <env>] [--spack-home <dir>] [--spack-reinstall] [--valgrind]"
                    exit 
        ;;
        --clang)
                    clang=1
                    shift
        ;;
        --debug)
                    debug="ON"
                    shift
        ;;
        --spack-env)
                    spack_env="$2"
                    shift
                    shift
        ;;
        --python)
                    with_python="ON"
                    shift
        ;;
        --python-exe)
                    python_exe="-DPython_EXECUTABLE=$2"
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
        --spack-reinstall)
                    spack_reinstall=1
                    shift
        ;;
        --valgrind)
                    with_valgrind="valgrind"
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
if [[ "$spack_dev" -eq 0 ]]; then
    \rm -Rf _spack_tpls
    git clone https://github.com/or-fusion/or-fusion-spack-repo.git _spack_tpls
else
    git clone git@github.com:or-fusion/or-fusion-spack-repo.git _spack_tpls
    cd _spack_tpls
    git checkout dev
    git pull
    cd ..
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
    git clone https://github.com/spack/spack.git ${SPACK_HOME}
    . ${SPACK_HOME}/share/spack/setup-env.sh
    spack repo add _spack_tpls/repo
    spack repo list
    spack env create $spack_env
    spack env activate $spack_env
    spack add asl cppad fmt rapidjson catch2 highs $with_valgrind
    spack install
    spack env deactivate
    spack repo remove _spack_tpls/repo
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
cmake -DCMAKE_PREFIX_PATH=${SPACK_HOME}/var/spack/environments/${spack_env}/.spack-env/view -Dwith_debug=${debug} -Dwith_python=${with_python} $python_exe -Dwith_gurobi=$with_gurobi -Dwith_highs=ON -Dwith_cppad=ON -Dwith_fmtlib=ON -Dwith_rapidjson=ON -Dwith_catch2=ON -Dwith_tests=ON -Dwith_asl=ON -Dwith_openmp=OFF ..
make -j20
make install

