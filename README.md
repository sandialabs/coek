# Coek: A C++ Optimization Expression Kernel

![coverage](https://gitlab.com/coopr/coek/badges/master/coverage.svg)


The C/C++ library that supports the definition of expressions used to formulate and solve optimization problems.

## Simple Build

* mkdir build
* cd build
* cmake ..

## Build with Tests

* cd third\_party
* ./installCatch2.sh
* cd ..
* mkdir build
* cd build
* cmake -Dwith\_tests=ON ..
* make
* make test

## Build with Solvers and CppAD (for NLP solvers)

* cd third\_party
* ./installCatch2.sh
* ./installCppAD.sh
* cd ..
* mkdir build
* cd build
* cmake -Dwith\_tests=ON -Dwith\_cppad=ON -Dwith\_ipopt=ON -Dwith\_gurobi=ON ..
* make
* make test

## Build with Pybind11 Python extensions (build a WHEEL and install with PIP)

* cd third\_party
* ./installPybind11.sh
* cd ..
* mkdir build
* cd build
* cmake -Dwith\_pybind11=ON ..
* make
* make pip\_install\_pybind11

## Build with Cppyy Python extensions (build a WHEEL and install with PIP)

* conda create -n coek python=3 cmake
* conda activate coek
* conda install -y clang python-clang cppyy
* mkdir build
* cd build
* cmake -Dwith\_cppyy=ON ..
* make
* make pip\_install\_cppyy

## Build with documentation

* conda create -n coek python=3 cmake
* conda activate coek
* conda install -y doxygen sphinx breathe sphinx\_rtd\_theme
* mkdir build
* cd build
* cmake -Dwith\_docs=ON ..
* make

## Install

* cd build
* cmake -DCMAKE\_INSTALL\_PREFIX=${COEK\_HOME} ..
* make install


## Next Steps

* Direct expression constraint for faster Gurobi interface
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
