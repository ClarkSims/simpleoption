#!/usr/bin/env bash
# Remove all build directories.
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

rm -rf "$SCRIPT_DIR/build"
rm -rf "$SCRIPT_DIR/test/build"
rm -rf "$SCRIPT_DIR/demo/build"

echo "Clean complete."
