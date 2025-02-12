#!/bin/bash

# build_wasm.sh - Clean, build, and deploy WebAssembly project.
# Usage: ./build_wasm.sh [clean|build|deploy|all|help]
# Defaults to 'all' (clean, build, deploy) if no argument is provided.

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
    if emcc --clear-cache; then
        log "Cache cleared successfully."
    else
        error "Failed to clear emscripten cache."
    fi
}

build() {
    log "Building..."
    cmake --preset wasm || error "CMake configuration failed."
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
        error "No artifacts found matching pattern '$DEPLOY_PATTERN' in '$BUILD_DIR/client/'."
    fi
}

# Default to 'all' if no argument provided
ACTION="${1:-all}"

# Execute commands based on the provided argument
case "$ACTION" in
    clean) clean ;;
    build) build ;;
    deploy) deploy ;;
    all) clean && build && deploy ;;
    help | --help | -h) grep '^# ' "$0" | cut -c 3- ;;
    *) error "Unknown option: '$ACTION'. Use './build_wasm.sh help'." ;;
esac
