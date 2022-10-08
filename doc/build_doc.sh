#!/bin/bash -v
#
# build_doc.sh <PROJECT>
#

[ -d _readthedocs/. ] || mkdir _readthedocs

if [[ $1 == "coek" ]]; then
    echo "Creating conda environment";
    mamba env create -p rtd -f doc/environment.yml
    echo "Building documentation with cmake";
    mkdir _build
    cd _build
    ../rtd/bin/cmake -Dwith_docs=ON ..
    make sphinx
    #pwd
    #ls
    #ls doc
    #ls doc/sphinx
    #echo "UP"
    #ls ..
    mv lib/coek/doc/sphinx ../_readthedocs/html
fi
if [[ $1 == "poek" ]]; then
    pip install sphinx sphinx_rtd_theme
    echo "Building documentation with sphinx";
    cd lib/poek/doc
    make html
    mv _build/html ../../../_readthedocs
fi
