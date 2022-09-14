#!/bin/bash

if [[ $1 == "coek" ]]; then
    echo "Running doxygen";
    mkdir _build
    cd _build
    mamba activate rtd
    cmake -Dwith_docs=ON ../lib/coek
    make
    pwd
    ls
fi
if [[ $1 == "poek" ]]; then
    echo "Running doxygen";
fi
