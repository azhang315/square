#!/bin/bash

# build_wasm.sh - Modularized WebAssembly build script.
# Usage: ./build_wasm.sh [clean|preset|build|deploy|full|build-to-end|help]
# - full          : Clean, preset, build, deploy (End-to-end)
# - build-to-end  : Build, deploy (Skips preset & clean)
# - clean         : Removes build directory (Skips cache clearing)
# - preset        : Runs CMake preset configuration
# - build         : Compiles WebAssembly
# - deploy        : Deploys built artifacts

set -euo pipefail

BUILD_DIR="build/wasm"
DEPLOY_DIR="client/web/build"
DEPLOY_PATTERN="client.*"

log() { echo -e "\033[1;34m[INFO]\033[0m $1"; }
error() {
    echo -e "\033[1;31m[ERROR]\033[0m $1" >&2
    exit 1
}

clean() {
    log "Cleaning..."
    rm -rf "$BUILD_DIR" || error "Failed to remove build directory."
}

clear_cache() {
    log "Clearing Emscripten cache..."
    if emcc --clear-cache; then
        log "Cache cleared successfully."
    else
        error "Failed to clear Emscripten cache."
    fi
}

preset() {
    log "Running CMake preset..."
    cmake --preset wasm || error "CMake preset failed."
}

build() {
    log "Building..."
    cmake --build "$BUILD_DIR" || error "Build failed."
}

deploy() {
    log "Deploying..."
    ARTIFACTS=("$BUILD_DIR/client"/$DEPLOY_PATTERN)

    if compgen -G "$BUILD_DIR/client/$DEPLOY_PATTERN" > /dev/null; then
        mkdir -p "$DEPLOY_DIR"
        cp -f "${ARTIFACTS[@]}" "$DEPLOY_DIR" || error "Deployment failed."
        log "Deployment complete: Artifacts copied to '$DEPLOY_DIR'."
    else
        error "No artifacts found matching pattern '$DEPLOY_PATTERN' in '$BUILD_DIR/client/'"
    fi
}

# Default to 'full' if no argument provided
ACTION="${1:-full}"

case "$ACTION" in
    clean) clean ;;
    preset) preset ;;
    build) build ;;
    deploy) deploy ;;
    all) clean && clear_cache && preset && build && deploy ;;
    full) clean && preset && build && deploy ;;
    help | --help | -h) grep '^# ' "$0" | cut -c 3- ;;
    *) error "Unknown option: '$ACTION'. Use './build_wasm.sh help'." ;;
esac
