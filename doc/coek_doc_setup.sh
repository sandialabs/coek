#!/bin/bash

mamba create -p rtd -f doc/environment.yml
conda activate `pwd`/rtd

if [[ $1 == "coek" ]]; then
    echo "Running doxygen";
    mkdir _build
    cd _build
    cmake -Dwith_docs=ON ../lib/coek
    make
    pwd
    ls
fi
if [[ $1 == "poek" ]]; then
    echo "Running doxygen";
fi
