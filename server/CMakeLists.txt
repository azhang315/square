cmake_minimum_required(VERSION 3.14)
project(Server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Recursively collect all .cpp source files in src/ (and subdirectories).
file(GLOB_RECURSE SERVER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(server ${SERVER_SOURCES})

# Add your local include directories.
target_include_directories(server PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link against the shared static library.
target_link_libraries(server PRIVATE shared)

# Find dependencies via vcpkg (make sure the vcpkg toolchain is active).
find_package(spdlog CONFIG REQUIRED)

# Link Boost and spdlog. (pthread is added here as needed for Boost on Linux.)
target_link_libraries(server PRIVATE spdlog::spdlog pthread)
