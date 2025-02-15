#!/bin/bash
#
# Benchmark comparison
#
# This script should be run from the top-level _build directory
#

keep_results=0
while [[ $# -gt 0 ]]; do
    case $1 in
        --help)
                    echo "bench_gurobi.sh [--help] [--keep]"
                    exit
        ;;
        --keep)
                    keep_results=1
                    shift
        ;;
    esac
done

if [[ -d /coek_performance_results ]]; then
    echo $CI_COMMIT_SHA
    echo $CI_COMMIT_TITLE
fi

if [[ "${keep_results}" -eq 0 ]]; then
    \rm -Rf results
fi
../test/aml_comparisons/scripts/run bench_coek 3
../test/aml_comparisons/scripts/collect
../test/aml_comparisons/scripts/dog solve0
../test/aml_comparisons/scripts/dog writer

if [[ -d /coek_performance_results ]]; then
    CI_COMMIT_TITLE_MOD="${CI_COMMIT_TITLE// /_}"
    python ../test/aml_comparisons/scripts/to_csv.py --dirname /coek_performance_results/${BRANCH_NAME} --branch_name ${BRANCH_NAME} --ci_commit_sha    ${CI_COMMIT_SHA} --ci_commit_title ${CI_COMMIT_TITLE_MOD}
    python ../test/aml_comparisons/scripts/compare.py --artifact_dir=/coek_performance_results --branch_name=${BRANCH_NAME}
else
    echo ""
    echo "Compiling results into a CSV file"
    echo ""
    python ../test/aml_comparisons/scripts/to_csv.py --dirname results --branch_name "CURRENT" --ci_commit_sha "SHA" --ci_commit_title "TITLE"
    echo "... done."
    echo ""
    echo "Comparing results"
    echo ""
    python ../test/aml_comparisons/scripts/compare.py --artifact_dir=results --branch_name="CURRENT"
    echo "... done."
fi

