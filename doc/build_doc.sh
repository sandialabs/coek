#!/bin/bash -v
#
# build_doc.sh <PROJECT>
#

[ -d _readthedocs/. ] || mkdir _readthedocs

#
# Build lib/coek documentation
#
if [[ $1 == "coek" ]]; then
    echo "Creating conda environment";
    mamba env create -p rtd -f doc/environment.yml
    echo "Building coek documentation with cmake";
    mkdir _build
    cd _build
    ../rtd/bin/cmake -Dwith_docs=ON ..
    make doc_coek
    mv lib/coek/doc/sphinx ../_readthedocs/html
fi

#
# Build lib/poek documentation
#
if [[ $1 == "poek" ]]; then
    echo "Creating conda environment";
    mamba env create -p rtd -f doc/environment.yml
    echo "Building poek documentation with cmake";
    mkdir _build
    cd _build
    conda run ../rtd/bin/cmake -Dwith_docs=ON -Dwith_python=ON ..
    make install_tpls
    make
    make doc_poek
    mv lib/poek/doc/sphinx ../_readthedocs/html
fi
