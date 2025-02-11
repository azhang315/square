# add_library(shared STATIC
#     include/shared/event.h
#     include/shared/event_dispatch.h
# )

# # Include the shared headers
# target_include_directories(shared PUBLIC include)

# # Expose both event.h and event_dispatch.h
# install(FILES 
#     include/shared/event.h
#     include/shared/event_dispatch.h
#     DESTINATION include/shared
# )
add_library(shared STATIC
    include/shared/event.h
    include/shared/event_dispatch.h
)

target_include_directories(shared PUBLIC include)

# Adjust settings for Emscripten if needed
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".js")  # Apply for client builds
endif()
