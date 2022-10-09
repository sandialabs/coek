[![Github Actions Status](https://github.com/sandialabs/coek/workflows/Linux%20Build%20and%20Tests/badge.svg?event=push)](https://github.com/sandialabs/coek/actions?query=workflow%3A%22Linux+Build+and+Tests%22++) 

# The Coek Project

## Overview

The goal of the Coek project is to develop performant optimization
modeling capabilities in both C++ and Python.  This is a monorepo that
contains the C++ Coek library, along with related C++ and Python libraries
that support fast interfaces to large, complex optimization applications.
Although the existing APIs for Coek and other libraries are stable,
these capabilities are expected to evolve significantly in response to
targetted experiments that demonstrate new strategies for performant
optimization modeling.


## Libraries

**Coek**: A fast C++ library that supports the definition of expressions used to formulate and solve optimization problems.

* [Online Documentation](http://coek.readthedocs.org/en/latest/)
[![Documentation Status](https://readthedocs.org/projects/coek/badge/?version=latest)](http://coek.readthedocs.org/en/latest/)

**pycoek**: A library of Python bindings for Coek.

**Poek**: A performant Python library that supports the definition of expressions used to formulate and solve optimization problems.

* [Online Documentation](http://poek.readthedocs.org/en/latest/)
[![Documentation Status](https://readthedocs.org/projects/poek/badge/?version=latest)](http://poek.readthedocs.org/en/latest/)


## Test Utilities

**aml_comparisons**: Performance tests for Coek, Poek and related optimization modeling environments.

* [README](test/aml_comparisons/README.md)


## Installation

### Simple Build

The Coek Project uses CMake to manage configuration of the build:

* mkdir build
* cd build
* cmake ..

The default installation directory is set to **build/install**.
Coek relies on a variety of third-party libraries, and CMake is setup to
automatically download, install and configure the use of these libraries:

* make install\_tpls

Third-party packages are installed in the installation directory.
This uses a CMAKE superbuild process that requires multiple executions
of the **make** command.

The default build logic for Coek is executed using `make`:

* make

By default, this does the following:

1. Build the Coek library.
   * If third-party libraries are automatically installed, this includes support for automatic differentiation using CppAD.
1. Build and install pycoek extensions.
   * If third-party libraries are automatically installed, this includes support for pybind11 extensions.
1. Install the Poek library in `editable` mode.
1. Build the aml\_comparisons performance testing utilities.

### Build with Tests

* mkdir build
* cd build
* cmake -Dwith\_tests=ON ..
* make intall_tpls
* make
* make test         # Test all libraries
* make test_coek    # Test the Coek library
* make test_poek    # Test the Poek library

### Download and build TPLs with curl insecure flags

* mkdir build
* cd build
* cmake -Dinsecure\_downloads=ON ..
* make install_tpls
* make

### Build with Gurobi

* mkdir build
* cd build
* cmake -Dwith\_gurobi=ON ..
* make install_tpls
* make

### Reformat code

* mkdir build
* cd build
* cmake ..
* make install_tpls
* make format-cpp-check     # Show C++ files that would be reformatted with clang-format
* make format-cpp           # Reformat C++ files with clang-format
* make format-py-check      # Show Python files that would be reformatted with black
* make format-py            # Reformat Python files with black

