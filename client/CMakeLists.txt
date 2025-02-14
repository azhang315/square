# client/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(Client)

# Gather sources
file(GLOB CLIENT_SOURCES src/*.cpp)
add_executable(client ${CLIENT_SOURCES})

# Include client headers
target_include_directories(client PRIVATE include)

target_link_libraries(client PRIVATE shared)
# find_package(spdlog CONFIG REQUIRED)
# target_link_libraries(client PRIVATE spdlog::spdlog_header_only)

# Set additional Emscripten build flags, preferably on the target itself
if(EMSCRIPTEN)
target_compile_options(client PRIVATE
    -O0
    -g3
    -gsource-map
    -Wall
    -Wextra
    -Wpedantic
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
    -pthread
    -matomics
    -mbulk-memory
    -sSHARED_MEMORY=1
    -sUSE_BOOST_HEADERS=1
)

target_link_options(client PRIVATE 
    -g3
    -sWASM=1
    -sASSERTIONS=2
    -sSAFE_HEAP=1
    # -sSAFE_HEAP_LOG=1
    -sSTACK_OVERFLOW_CHECK=2
    -sEXPORTED_RUNTIME_METHODS=[ccall,cwrap]
    -sEXPORTED_FUNCTIONS=[_main]
    # WebGL support
    -sUSE_WEBGL2=1
    -sMAX_WEBGL_VERSION=2
    -sMIN_WEBGL_VERSION=2
    -sFULL_ES3=1
    -sLEGACY_GL_EMULATION=0
    # -sOFFSCREEN_FRAMEBUFFER=1
    # -sOFFSCREENCANVAS_SUPPORT=1
    -sGL_DEBUG=1
    -lGL
    # Shared memory
    -pthread 
    -sPTHREADS_DEBUG
    -sSHARED_MEMORY=1
    -sPTHREAD_POOL_SIZE=4
    # POSIX socket API proxy
    -sPROXY_TO_PTHREAD=1
    -lwebsocket.js 
    -sPROXY_POSIX_SOCKETS=1
)
endif()
if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "EMSCRIPTEN must be enabled for this build! Please invoke CMake with -DEMSCRIPTEN=ON and the proper Emscripten toolchain.")
endif()
