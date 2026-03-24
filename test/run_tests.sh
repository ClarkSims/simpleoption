#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

cd "$BUILD_DIR"
cmake ..
make

TESTS=(
    test1.americanoption
    test1.simpleoption
    test2.americanoption
    test3.simpleoption
    test4.americanoption
    test4.simpleoption
    test.allgreeks
    test.allopt
    test.allvol
    test.american_option_approximation
    test.binomial_greeks
    test_cdf
)

for TEST in "${TESTS[@]}"; do
    echo "Running $TEST..."
    "$BUILD_DIR/$TEST"
    if [ $? -ne 0 ]; then
        echo "FAILED: $TEST"
        exit 1
    fi
done

echo "All Test Passed!"
