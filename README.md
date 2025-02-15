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
