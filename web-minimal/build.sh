#!/bin/bash

# Define directories
SRC_DIR=src
INCLUDE_DIR=include
BUILD_DIR=build

# Create build directory if it doesn't exist
mkdir -p $BUILD_DIR

# Compile the C++ sources to WASM
emcc $SRC_DIR/*.cpp -I$INCLUDE_DIR -o $BUILD_DIR/app.js \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s EXPORTED_RUNTIME_METHODS=[] \
    -s ASSERTIONS=1 \
    --no-entry \
    -O0

echo "Build complete. Files are in $BUILD_DIR/"
