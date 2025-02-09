#!/bin/bash

SRC_DIR=src
INCLUDE_DIR=include
BUILD_DIR=build

mkdir -p $BUILD_DIR

emcc $SRC_DIR/*.cpp -I$INCLUDE_DIR -o $BUILD_DIR/app.js \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s EXPORTED_RUNTIME_METHODS=[] \
    -s ASSERTIONS=1 \
    --no-entry \
    -O0 \
    -Wall -Wextra -Wpedantic \
    -Wshadow -Wconversion -Wsign-conversion -Wcast-align -Wundef -Wfloat-equal \
    -Wcast-qual -Wwrite-strings -Wuninitialized -Wredundant-decls -Wold-style-cast \
    # -Werr

echo "Build complete. Files are in $BUILD_DIR/"
