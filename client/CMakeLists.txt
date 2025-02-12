# client/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(Client)

# Gather client sources
file(GLOB CLIENT_SOURCES src/*.cpp)
add_executable(client ${CLIENT_SOURCES})

# Include your client’s headers (if any)
target_include_directories(client PRIVATE include)

# Link against the shared library target (set up elsewhere)
target_link_libraries(client PRIVATE shared)

# Find packages via vcpkg (which is already integrated via your toolchain file)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(client PRIVATE spdlog::spdlog)

# Set additional Emscripten build flags, preferably on the target itself
if(EMSCRIPTEN)
target_compile_options(client PRIVATE
    "-O0"
    "-Wall"
    "-Wextra"
    "-Wpedantic"
    # "-Wshadow"
    # "-Wconversion"
    # "-Wsign-conversion"
    # "-Wcast-align"
    # "-Wundef"
    # "-Wfloat-equal"
    # "-Wcast-qual"
    # "-Wwrite-strings"
    # "-Wuninitialized"
    # "-Wredundant-decls"
    # "-Wold-style-cast"
)
target_link_options(client PRIVATE 
    -sWASM=1
    -sALLOW_MEMORY_GROWTH=1
    -sASSERTIONS=1
    -sEXPORTED_RUNTIME_METHODS=[ccall,cwrap]
    -sEXPORTED_FUNCTIONS=[_main,_test_server_log]
    -sSTRICT=1
    -sUSE_WEBGL2=1
    -sMAX_WEBGL_VERSION=2
    -sMIN_WEBGL_VERSION=2
    -sFULL_ES3=1
    -sLEGACY_GL_EMULATION=0
    -sOFFSCREEN_FRAMEBUFFER=1
    -sGL_DEBUG=1
    # -sCOOP=1
    # -sCOEP=1
    -lGL
)
# target_link_options(client PRIVATE 
#     "-s WASM=1"
#     "-s ALLOW_MEMORY_GROWTH=1"
#     "-s ASSERTIONS=1"
#     "-s EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\"]'"
#     "-s EXPORTED_FUNCTIONS='[\"_main\",\"_test_server_log\"]'"
#     "-s STRICT"
#     "-lGL"
#     "-s USE_WEBGL2=1"
#     "-s MAX_WEBGL_VERSION=2"
#     "-s MIN_WEBGL_VERSION=2"
#     "-s LEGACY_GL_EMULATION=0"
#     "-s FULL_ES3=1"
#     "-s OFFSCREEN_FRAMEBUFFER=1"
#     "-s USE_BOOST_HEADERS=1"
#     "-s GL_DEBUG=1"
#     "-s COOP=1"
#     "-s COEP=1"
#     # "-pthread"
#     # "-s ENVIRONMENT=web,worker"
#     # "-s PTHREAD_POOL_SIZE=32"
# )
endif()
if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "EMSCRIPTEN must be enabled for this build! Please invoke CMake with -DEMSCRIPTEN=ON and the proper Emscripten toolchain.")
endif()
