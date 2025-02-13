# client/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(Client)

# Gather sources
file(GLOB CLIENT_SOURCES src/*.cpp)
add_executable(client ${CLIENT_SOURCES})

# Include client headers
target_include_directories(client PRIVATE include)

target_link_libraries(client PRIVATE shared)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(client PRIVATE spdlog::spdlog_header_only)

# Set additional Emscripten build flags, preferably on the target itself
if(EMSCRIPTEN)
target_compile_options(client PRIVATE
    -O0
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
    -sUSE_PTHREADS=1
    -matomics
    -mbulk-memory
    -sSHARED_MEMORY=1
)
target_link_options(client PRIVATE 
    -sWASM=1
    -sALLOW_MEMORY_GROWTH=1
    -sASSERTIONS=1
    -sEXPORTED_RUNTIME_METHODS=[ccall,cwrap]
    -sEXPORTED_FUNCTIONS=[_main,_test_server_log]
    # -sSTRICT=1
    -sUSE_WEBGL2=1
    -sMAX_WEBGL_VERSION=2
    -sMIN_WEBGL_VERSION=2
    -sFULL_ES3=1
    -sLEGACY_GL_EMULATION=0
    -sOFFSCREEN_FRAMEBUFFER=1
    -sGL_DEBUG=1
    # -sCOOP=1
    # -sCOEP=1

    # Explicit
    -lGL
    # Shared memory
    -pthread 
    -sUSE_PTHREADS=1
    -sSHARED_MEMORY=1

    # POSIX socket proxying
    -lwebsocket.js 
    -sPROXY_POSIX_SOCKETS=1
    -sPROXY_TO_PTHREAD=1
)
endif()
if(NOT EMSCRIPTEN)
  message(FATAL_ERROR "EMSCRIPTEN must be enabled for this build! Please invoke CMake with -DEMSCRIPTEN=ON and the proper Emscripten toolchain.")
endif()
