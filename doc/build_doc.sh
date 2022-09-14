#!/bin/bash -v
#
# build_doc.sh <PROJECT>
#

mamba env create -p rtd -f doc/environment.yml
#conda activate `pwd`/rtd

mkdir -f _readthedocs

if [[ $1 == "coek" ]]; then
    echo "Running doxygen";
    mkdir _build
    cd _build
    ../rtd/bin/cmake -Dwith_docs=ON ../lib/coek
    make sphinx
    #pwd
    #ls
    #ls doc
    #ls doc/sphinx
    mv doc/sphinx ../_readthedocs/html
fi
if [[ $1 == "poek" ]]; then
    echo "Running doxygen";
fi
