#!/bin/bash

# build_x64.sh - Modularized native build script (x64-linux).
# Usage: ./build_x64.sh [clean|preset|build|deploy|full|build-to-end|help]
# - full          : Clean, preset, build (End-to-end)
# - build-to-end  : Build (Skips preset & clean)
# - clean         : Removes x64 build directory
# - preset        : Runs CMake preset configuration
# - build         : Compiles the native binary
# - deploy        : (Optional) Placeholder for future native deployment

set -euo pipefail

BUILD_DIR="build/x64-linux"

log() { echo -e "\033[1;34m[INFO]\033[0m $1"; }
error() {
    echo -e "\033[1;31m[ERROR]\033[0m $1" >&2
    exit 1
}

clean() {
    log "Cleaning x64-linux build directory..."
    rm -rf "$BUILD_DIR" || error "Failed to remove build directory."
}

preset() {
    log "Running CMake preset for x64-linux..."
    cmake --preset x64-linux || error "CMake preset failed for x64-linux."
}

build() {
    log "Building x64-linux binary..."
    cmake --build "$BUILD_DIR" || error "Build failed for x64-linux."
}

deploy() {
    log "Native build does not require deployment. Skipping..."
}

# Default action is 'full' if no argument is provided
ACTION="${1:-full}"

case "$ACTION" in
    clean) clean ;;
    preset) preset ;;
    build) build ;;
    deploy) deploy ;;
    all) clean && preset && build ;;
    full) preset && build ;;
    help | --help | -h) grep '^# ' "$0" | cut -c 3- ;;
    *) error "Unknown option: '$ACTION'. Use './build_x64.sh help'." ;;
esac
