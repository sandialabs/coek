# Coek: A C++ Optimization Expression Kernel

![coverage](https://gitlab.com/coopr/coek/badges/master/coverage.svg)


The C/C++ library that supports the definition of expressions used to formulate and solve optimization problems.

## Build

* cd third\_party
* ./getCatch2.sh
* cd ..
* mkdir build
* cd build
* cmake -Dwith\_tests=ON ..
* make
* make test


## Install

* cd build
* cmake -DCMAKE\_INSTALL\_PREFIX=../\_install ..
* make install

