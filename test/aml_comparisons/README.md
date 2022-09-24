# aml_comparisons

Performance tests for Coek, Poek and related optimization modeling environments.

## Running tests

From the build directory, you can run the *run* script to generate 10 trials for each test:

* ../scripts/run bench_writer 10
* ../scripts/run bench_solve0 10
* ../scripts/collect
* ../scripts/dog writer
* ../scripts/dog solve0

See the *run* script, at the end, for pre-canned tests.  Tests
results are stored in the results/writer and results/solve0 directories.

The *collect* script creates JSON files that summarize these results, and these JSON files are used by the *dog* script
to create reports and graphs.

