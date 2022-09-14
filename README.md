[![Github Actions Status](https://github.com/sandialabs/coek/workflows/Linux%20Build%20and%20Tests/badge.svg?event=push)](https://github.com/sandialabs/coek/actions?query=event%3Apush+workflow%3A%Linux+Build+and+Tests%22)

# The Coek Project

## Overview

The goal of the Coek project is to develop performant optimization
modeling capabilities in both C++ and Python.  This is a monorepo that
contains the C++ Coek library, along with related C++ and Python libraries
that support fast interfaces to large, complex optimization applications.
Although the existing APIs for Coek and other libraries are stable,
these capabilities are expected to evolve significantly in response to
targetted experiments that demonstrate new strategies
for performant optimization modeling.


## Libraries

**Coek**: A fast C++ library that supports the definition of expressions used to formulate and solve optimization problems.

* [README](lib/coek/README.md)
* [Online Documentation](http://coek.readthedocs.org/en/latest/): ![Documentation Status](https://readthedocs.org/projects/coek/badge/?version=latest)

**Poek**: A performant Python library that supports the definition of expressions used to formulate and solve optimization problems.

* [README](lib/poek/README.md)


## Test Utilities

**aml_comparisons**: Performance tests for Coek, Poek and related optimization modeling environmants.

* [README](test/aml_comparisons/README.md)
