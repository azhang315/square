# server/CMakeLists.txt
cmake_minimum_required(VERSION 3.14)
project(Server)

# Server sources
file(GLOB SERVER_SOURCES src/*.cpp src/network/*.cpp src/core/*.cpp)

# Add executable
add_executable(server ${SERVER_SOURCES})

# Link against shared library
target_include_directories(server PRIVATE include)
target_link_libraries(server PRIVATE shared)

# Include directories
target_include_directories(server PRIVATE include)

# Boost Asio (Header-only)
find_package(Boost REQUIRED COMPONENTS system)
target_link_libraries(server PRIVATE Boost::system pthread)

# spdlog for logging
find_package(spdlog REQUIRED)
target_link_libraries(server PRIVATE spdlog::spdlog)
