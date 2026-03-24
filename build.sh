#!/usr/bin/env bash
# Build everything as a normal user.
# Builds the library, all tests, and all demos into ./build/
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

mkdir -p "$SCRIPT_DIR/build"
cd "$SCRIPT_DIR/build"
cmake ..
make -j$(nproc)

echo ""
echo "Build complete."
echo "  To run tests:  ./build_and_test.sh"
echo "  To install:    sudo ./install.sh"
