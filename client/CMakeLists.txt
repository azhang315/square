cmake_minimum_required(VERSION 3.14)
project(Client)

# Emscripten-specific flags
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Client sources
file(GLOB CLIENT_SOURCES src/*.cpp)
add_executable(client ${CLIENT_SOURCES})

# Link against shared library
target_include_directories(client PRIVATE include)
target_link_libraries(client PRIVATE shared)

# Find packages (handled by vcpkg)
find_package(spdlog CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# Link libraries
target_link_libraries(client PRIVATE spdlog::spdlog Boost::system)


# Link Boost headers (header-only)
target_include_directories(client PRIVATE ${boost_asio_SOURCE_DIR}/include ${boost_system_SOURCE_DIR}/include)
target_link_libraries(client PRIVATE Boost::system)

# Emscripten build flags
if(EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s EXPORTED_FUNCTIONS=['_main','_test_server_log']")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s FULL_ES3=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s OFFSCREEN_FRAMEBUFFER=1 -s GL_DEBUG=1 -pthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s ENVIRONMENT=web,worker -s PTHREAD_POOL_SIZE=32")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -Wpedantic")
endif()



