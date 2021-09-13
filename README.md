# Coek: A C++ Optimization Expression Kernel

![coverage](https://gitlab.com/coopr/coek/badges/master/coverage.svg)


The C/C++ library that supports the definition of expressions used to
formulate and solve optimization problems.

## Setup and Configuration

By default, Coek builds just its library and it assumes that third-party
packages are installed and available.  The default installation directory
is set to **build/install**.

Optionally, a user can used the **build\_\* ** options to build
third-party packages locally. The packages are installed in the
installation directory along with Coek.  This uses a CMAKE superbuild
process that requires multiple executions of the **make** command.
This is convenient for Coek developers, but installation with a package
manager is probably a better long-term solution for users.

## Build Examples

### Simple Build

* mkdir build
* cd build
* cmake ..

### Build TPLs and Tests

* mkdir build
* cd build
* cmake -Dbuild\_all=ON -Dwith\_tests=ON ..
* make tpls
* make
* make test

### Download and build TPLs with curl insecure flags

* mkdir build
* cd build
* cmake -Dbuild\_all=ON -Dinsecure\_downloads=ON -Dwith\_tests=ON ..
* make tpls
* make
* make test

### Build with Solvers and CppAD (for NLP solvers)

* mkdir build
* cd build
* cmake -Dbuild\_catch2=ON -Dbuild\_cppad=ON -Dwith\_tests=ON -Dwith\_ipopt=ON -Dwith\_gurobi=ON ..
* make tpls
* make
* make test

### Build with Pybind11 Python extensions (build a WHEEL and install with PIP)

* mkdir build
* cd build
* cmake -Dbuild\_pybind11=ON -Dwith\_pybind11=ON ..
* make
* make pip\_install\_pybind11

### Build with Cppyy Python extensions (build a WHEEL and install with PIP)

* conda create -n coek python=3 cmake
* conda activate coek
* conda install -y clang python-clang cppyy
* mkdir build
* cd build
* cmake -Dwith\_cppyy=ON ..
* make
* make pip\_install\_cppyy

### Build with documentation

* conda create -n coek python=3 cmake
* conda activate coek
* conda install -y doxygen sphinx breathe sphinx\_rtd\_theme
* mkdir build
* cd build
* cmake -Dwith\_docs=ON ..
* make


### Install

By default, coek is installed in the build/install directory:

* mkdir build
* cd build
* cmake ..
* make
* make install


## Next Steps

In no particular order...

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
