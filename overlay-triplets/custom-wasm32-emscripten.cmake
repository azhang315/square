# custom-triplets/custom-wasm32-emscripten.cmake

# Optionally include the base triplet settings if desired:
include("${VCPKG_ROOT_DIR}/triplets/community/wasm32-emscripten.cmake")

# Append Emscripten-specific flags
set(VCPKG_CXX_FLAGS "${VCPKG_CXX_FLAGS} -pthread -sUSE_PTHREADS=1 -sATOMICS=1 -matomics -mbulk-memory sSHARED_MEMORY=1")
set(VCPKG_C_FLAGS   "${VCPKG_C_FLAGS}   -pthread -sUSE_PTHREADS=1 -sATOMICS=1 -matomics -mbulk-memory sSHARED_MEMORY=1")
set(VCPKG_LINKER_FLAGS "${VCPKG_LINKER_FLAGS} -pthread -sUSE_PTHREADS=1 -sATOMICS=1 -matomics -mbulk-memory sSHARED_MEMORY=1")

# Propagate to CMake variables
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VCPKG_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VCPKG_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${VCPKG_LINKER_FLAGS}")

message(STATUS "Custom CXX Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Custom C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "Custom Linker Flags: ${CMAKE_EXE_LINKER_FLAGS}")
