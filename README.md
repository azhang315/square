# Collaborative Canvas
## Dependencies
[vcpkg](https://github.com/microsoft/vcpkg) quick setup example:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
```
For setting `VCPKG_ROOT` for CMake:
```bash
export VCPKG_ROOT=/path/to/vcpkg
echo $VCPKG_ROOT
```
## Build
Two CMake presets defined:
- wasm: for WASM client build
- x64-linux: for native server build

### Build Native Server
   ```bash
   cmake --preset x64-linux
   cmake --build build/native
   ```
### Build WASM Client
   ```bash
   cmake --preset wasm
   cmake --build build/wasm
   ```
   Optionally, move the generated `.js` and `.wasm` files to your web server's root. For example:

   ```bash
   mv build-wasm/*.js build-wasm/*.wasm client/web/build
   ```

    Serve the Client:
   ```bash
   cd client/web/build
   python3 -m http.server 8080
   ```