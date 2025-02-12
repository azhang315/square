# shared/CMakeLists.txt
add_library(shared INTERFACE)

target_include_directories(shared INTERFACE include)

# Adjust settings for Emscripten if needed
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")  # Apply for client builds
endif()