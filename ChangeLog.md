# ChangeLog

Here we list changes of Coek.  More detailed information
about incremental changes can be found in the [commit
history](https://github.com/sandialabs/coek/commits).

## 1.4

* Added support for multithreaded model creation.
* Fixed bug with ASL missing constraint bounds
* Added option logic to autograd class hierarchy
* Added option to specify sparse Jacobian in CppAD interface
* Added option to specify temporary directory for ASL AD interface
* Added option to remove temporary NL file in ASL AD interface
* Generalized performance comparison graphs to use dates for the x-axis

## 1.3

* Added ASL autograd functionality
* Various fixes in expression management logic.
* Improving support for C++14
* Updates to support Python 3.11
* Switch to use SHA256 hashes for third-party packages
* Adding expression simplifier logic
* Fixing many bugs with NL writer

## 1.2

* Added the SubExpression object, which is used to denote 
* Revised the mutable_values() and find_vars_and_params() functions to avoid re-evaluating SubExpression objects.
* Added the  evaluate_expr() function, which avoids re-evaluating SubExpression objects.
* Fixed errors in the ipopt interface; constraint bounds were not being propigated properly.
* Tests added to confirm a fix in CppAD for expressions of the form 0/f(x).
* Fix bugs with walkers, to ensure that PlusTerm objects are only iterated over their prefix
* Fixing bug in symbolic differentation.  Avoid double-counting when computing the in-degree.
* Simplified the operators in the core COEK API classes.
* Adding operators to avoid Variable->Expression coercion, which could happen frequently in practice.
* Reworked sequence operators and extended this functionality to include the incides() function.
* Major edits to use std::shared_ptr rather than explicit reference counting in Coek.
* Added explicit management of variable naming as simple/lazy/eager.
* Performance improvements via RVO.
* Adding graphs to facilitate tracking of performance changes.

## 1.1.1

* Revised build logic documentation.
* Enabled ReadTheDocs documentation for both Coek and Poek.

## 1.1.0

* Revised CMake logic to support an integrated build of this monorepo.
* Adding make targets to format C++ and Python software.
* Fixes to Gurobi interface
* Revised github actions to test different configurations (e.g. with/out Python)

## 1.0

Initial public release.
