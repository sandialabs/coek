# coek_testmodels

## Building

First, build COEK in a peer directory.  Then, create a build directory here:

* mkdir build
* cd build
* cmake ..

## Running tets

From the build directory, you can run the test_build.py script:

* python ../scripts/test_build.py bench_writer
* python ../scripts/test_build.py bench_solve0
* ../scripts/collect
* ../scripts/dog writer
* ../scripts/dog solve0

See the test_build.py script, at the end, for pre-canned tests.  Tests
results are stored in the results/writer and results/solve0 directories.

