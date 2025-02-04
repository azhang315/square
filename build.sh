#!/bin/bash

source ~/emsdk/emsdk_env.sh  # Load Emscripten environment

emcc webgpu_canvas.cpp -o webgpu_canvas.js \
    -s WASM=1 \
    -s USE_WEBGL=1 \

    # shared memory
    -s USE_PTHREADS=1 \
    -s PTHREAD_POOL_SIZE=4 \
    #
    
    -s EXPORTED_FUNCTIONS="['_setPixel', '_clearCanvas', '_getPixelBuffer', '_startMainLoop']" \
    -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" \
    --shell-file shell.html