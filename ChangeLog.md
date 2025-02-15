# ChangeLog

Here we list changes of Coek.  More detailed information
about incremental changes can be found in the [commit
history](https://github.com/sandialabs/coek/commits).

## 2.1

* Reworked Spack to use the or-fusion-spack-repo to define TPLs
* coek: Expanded list of domain names used to specify domain and bounds values
* coek: Updates to gurobi configuration and environment setup logic
* coek: Removed void* references and used shared pointer references instead
* coek: Reverted IndexVector to std::vector to resolve memory management issues
* coek: Refactored assoc_array logic to avoid cross-links within coek
* coek: Added Data logic, which represents immutable model data that is used in compact expressions
* coek: Added IndexSequence, which iterates over the IndexParameters associated with a ConcreteSet.  This provides a more generic iterator concept than ExpressionSequence.
* coek: Reworking map logic to use IndexSequence objects.  
* coek: Fixes to compact expression walkers.
* coek: Enabled expand() logic for Parameter's 
* coek: Fixed bugs with compact LP writer
* pycoek: Enable setup of data/param/var data with SequenceContext objects
* pycoek: Expose DataPortal
* tests: More robust testing scripts
* tests: Enable comparisons with coek as a baseline
* tests: Treating Gurobi license issues as warnings

## 2.0

* Adding support for builds with Spack
* Rework of CMake specification of coek::coek to account for subpackage dependencies
* coek_utils: Python utilities that work with coek
* coek: Made strict inequalities a special case, to simplify core objects
* coek: Reduced size of several core objects
* coek: Optimized initialization of associative array objects
* coek: Added support for model names
* coek: Added coek-specific exceptions to tailor use of coek visitors
* coek: Fixes for clang compiler
* coek: Adding support for activate/deactivate in objectives and constraints
* coek: Added tictoc and DataPortals logic
* coek: Added new template type utilities
* coek: Using std::to_string to standardize coek tests
* coek: Suppress Gurobi license server output by default
* coek: Added interface to Highs solver
* coek: Added support for detailed solver results information
* coek: Using size_t for unsigned integer values
* coek: Deprecating C++14 tests (for now)
* poek: Test updates based on standardized coek test outputs
* pycoek: Various API additions to improve pyomo_coek compatibility with pyomo
* pyomo_coek: Various changes to improve compatibility with pyomo
* pyomo_coek: Added tests for compatibility with pyomo

## 1.4

* Added support for multithreaded model creation.
* Fixed bug with ASL missing constraint bounds
* Added option logic to autograd class hierarchy
* Added option to specify sparse Jacobian in CppAD interface
* Added option to specify temporary directory for ASL AD interface
* Added option to remove temporary NL file in ASL AD interface
* Fixed model resolve logic to avoid explicit calls to reset()
* Consolidated option management
* Added if_else() function
* Consolidated automatic differentiation tests
* Generalized performance comparison graphs to use dates for the x-axis
* Updated performance plots showing relative performance
* Reworked expression walkers to remove use of visitor logic

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
