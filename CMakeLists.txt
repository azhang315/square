cmake_minimum_required(VERSION 3.14)
project(CollaborativeCanvas LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-fcolor-diagnostics)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include the shared library.
add_subdirectory(shared)


# Conditionally add subdirectories for server or client.
if(EMSCRIPTEN)
    add_subdirectory(client)
else()
    add_subdirectory(server)
    add_subdirectory(tools/websocket_to_posix_proxy) # POSIX proxy
endif()


