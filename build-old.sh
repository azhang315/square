# #!/bin/bash

# source ~/emsdk/emsdk_env.sh  # Load Emscripten environment

# emcc webgpu_canvas.cpp -o webgpu_canvas.js \
#     -s WASM=1 \
#     -s USE_WEBGL2=1 \

#     # shared memory
#     -s USE_PTHREADS=1 \
#     -s PTHREAD_POOL_SIZE=4 \
#     #
    
#     -s EXPORTED_FUNCTIONS="['_setPixel', '_clearCanvas', '_getPixelBuffer', '_startMainLoop']" \
#     -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" \
#     --shell-file shell.html


#!/bin/bash

# Set the output directory and files
OUT_DIR="dist"
OUT_JS="app.js"
OUT_WASM="app.wasm"

# Create the output directory if it doesn't exist
mkdir -p $OUT_DIR

# Emscripten build command
emcc src/*.cpp -Iinclude \
    -O3 \                                # Aggressive optimization
    -s WASM=1 \                          # Output WebAssembly
    -s MODULARIZE=1 \                    # Export as a JS module
    -s EXPORT_ES6=1 \                    # Use ES6 exports (cleaner in modern JS)
    -s ALLOW_MEMORY_GROWTH=1 \           # Allow memory growth dynamically
    -s NO_EXIT_RUNTIME=1 \               # Keep runtime alive for main loop
    -s EXPORTED_RUNTIME_METHODS='["cwrap", "ccall"]' \  # Expose C API to JS
    -s ASSERTIONS=0 \                    # Remove runtime assertions (release mode)
    -s INITIAL_MEMORY=64MB \             # Initial memory allocation
    --preload-file assets \              # Bundle any assets (if needed)
    -std=c++17 \                         # Use modern C++
    -o $OUT_DIR/$OUT_JS                  # Output JavaScript & WASM

echo "✅ Build complete: $OUT_DIR/$OUT_JS and $OUT_DIR/$OUT_WASM"
