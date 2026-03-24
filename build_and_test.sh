#!/usr/bin/env bash
# Build everything and run all tests. Run as a normal user.
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p "$SCRIPT_DIR/build"
cd "$SCRIPT_DIR/build"
cmake ..
make -j$(nproc)

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

echo ""
for TEST in "${TESTS[@]}"; do
    echo "Running $TEST..."
    "$SCRIPT_DIR/build/test/$TEST"
done

echo ""
echo "All tests passed!"
