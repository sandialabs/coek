#!/bin/bash
#
# Benchmark comparison
#
# This script should be run from the top-level _build directory
#

if [[ -d /coek_performance_results ]]; then
    echo $CI_COMMIT_SHA
    echo $CI_COMMIT_TITLE
fi

\rm -Rf results
../test/aml_comparisons/scripts/run ${1}:lp_writer 3
../test/aml_comparisons/scripts/run $1:solve0 3
../test/aml_comparisons/scripts/collect

echo ""
echo "SOLVE0 RESULTS"
echo ""

../test/aml_comparisons/scripts/dog solve0
../test/aml_comparisons/scripts/dog --statistic Ratio --baseline gurobi solve0
../test/aml_comparisons/scripts/dog --statistic Ratio --baseline coek solve0

echo ""
echo "WRITER RESULTS"
echo ""

../test/aml_comparisons/scripts/dog writer
../test/aml_comparisons/scripts/dog --statistic Ratio --baseline gurobi writer
../test/aml_comparisons/scripts/dog --statistic Ratio --baseline coek writer

if [[ -d /coek_performance_results ]]; then
    CI_COMMIT_TITLE_MOD="${CI_COMMIT_TITLE// /_}"
    python ../test/aml_comparisons/scripts/to_csv.py --dirname /coek_performance_results/${BRANCH_NAME} --branch_name ${BRANCH_NAME} --ci_commit_sha    ${CI_COMMIT_SHA} --ci_commit_title ${CI_COMMIT_TITLE_MOD}
    python ../test/aml_comparisons/scripts/compare.py --artifact_dir=/coek_performance_results --branch_name=${BRANCH_NAME}
fi

