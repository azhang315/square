#!/bin/bash

SRC_DIR="src"
INCLUDE_DIR="include"
BUILD_DIR="web/build"

SPDLOG_DIR="$HOME/dev/spdlog"  # Path to spdlog

mkdir -p $BUILD_DIR

emcc $SRC_DIR/*.cpp -I$INCLUDE_DIR -I$SPDLOG_DIR/include -o $BUILD_DIR/app.js \
    -s WASM=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=1 \
    -s EXPORTED_FUNCTIONS='[_main, _test_server_log]' \
    -s USE_WEBGL2=1 \
    -s MIN_WEBGL_VERSION=2 \
    -s FULL_ES3=1 \
    -s OFFSCREEN_FRAMEBUFFER=1 \
    -s GL_DEBUG=1 \
    -O0 \
    -Wall -Wextra -Wpedantic \
    -Wshadow -Wconversion -Wsign-conversion -Wcast-align -Wundef -Wfloat-equal \
    -Wcast-qual -Wwrite-strings -Wuninitialized -Wredundant-decls -Wold-style-cast \
    # -Werr

echo "Build complete. Files are in $BUILD_DIR/"
