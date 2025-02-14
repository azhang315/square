# # shared/CMakeLists.txt
# add_library(shared INTERFACE)

# target_include_directories(shared INTERFACE include)

# # Adjust settings for Emscripten if needed
# if(EMSCRIPTEN)
#     set(CMAKE_EXECUTABLE_SUFFIX ".js")  # Apply for client builds
# endif()

# # Create a STATIC library instead of INTERFACE
# add_library(shared STATIC)

# # Add both header and source directories
# target_include_directories(shared PUBLIC include)
# target_sources(shared PRIVATE src/log.cpp)

# Use an INTERFACE library since it's mostly header-only
add_library(shared INTERFACE)

# Ensure headers are accessible from both shared and client
target_include_directories(shared INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${spdlog_INCLUDE_DIRS}  # Ensure spdlog headers are available
)

# Ensure spdlog is found
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(shared INTERFACE spdlog::spdlog_header_only)


# Adjust settings for Emscripten if needed
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")  # Apply for client builds
endif()
