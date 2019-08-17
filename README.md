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


## Build with CppAD

* cd third\_party
* ./getCatch2.sh
* ./installCppAD.sh
* cd ..
* mkdir build
* cd build
* cmake -Dwith\_tests=ON -Dwith\_cppad=ON ..
* make
* tests/runner


## Install

* cd build
* cmake -DCMAKE\_INSTALL\_PREFIX=../\_install ..
* make install

## Next Steps

* Integrate Sacado ... and maybe the Stan Math Library
* Add support for interrogating the solver solution properties
* Add support for row/col properties (e.g. dual information)
* Add support for constraint generation (as a call-back)
* Q: How support model transformations (e.g. relaxing integratility)
* Add other solvers (What is the priority?)
* Demonstrate support for conic problems (and add conic solver interfaces)
* Interface with Poek 2.0
* Add SOS, MPEC, etc logic
* Add cut generation call-backs
* Add vector/matrix expression elements
