#!/bin/bash -v

mamba env create -p rtd -f doc/environment.yml
#conda activate `pwd`/rtd

mkdir -f _readthedocs

if [[ $1 == "coek" ]]; then
    echo "Running doxygen";
    mkdir _build
    cd _build
    ../rtd/bin/cmake -Dwith_docs=ON ../lib/coek
    make docs
    pwd
    ls
    ls docs
    mv docs/sphinx/html ../_readthedocs
fi
if [[ $1 == "poek" ]]; then
    echo "Running doxygen";
fi
