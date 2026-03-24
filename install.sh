#!/usr/bin/env bash
# Install the library. Run as root or with sudo.
# Build first with ./build.sh (as a normal user) before running this.
set -e

if [ "$(id -u)" -ne 0 ]; then
    echo "error: run as root or with sudo"
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ ! -f "$SCRIPT_DIR/build/libsimpleoption.so" ]; then
    echo "error: library not built yet. Run ./build.sh first (as a normal user)."
    exit 1
fi

make -C "$SCRIPT_DIR/build" install

echo "Install complete."
