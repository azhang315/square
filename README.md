# Dirtree
```
collaborative-canvas/
├── build-native/    # For server (native)
└── build-wasm/      # For client (WASM)
```

# External
```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install

(native)
./vcpkg install spdlog boost-system

(wasm)
./vcpkg install spdlog boost-system --triplet=wasm32-emscripten

(or based on vcpkg.json)
./vcpkg install
./vcpkg install --triplet=wasm32-emscripten
```


# Env
export VCPKG_ROOT=/path/to/vcpkg
echo $VCPKG_ROOT

or 

echo 'export VCPKG_ROOT=/path/to/vcpkg' >> ~/.bashrc
source ~/.bashrc

# Build native server
```
cd build-native
cmake ..
cmake --build . --target shared
cmake --build . --target server
./server/server
```

# Build WASM client
```
cd build-wasm
emcmake cmake ..
cmake --build . --target shared
cmake --build . --target client

# Move the build artifacts to the web root if needed
cd ~
mv build-wasm/*.js build-wasm/*.wasm client/web/build

# Serve the client using Python's HTTP server
cd client/web/build
python3 -m http.server 8080
```