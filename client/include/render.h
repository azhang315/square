#pragma once
#include <GLES3/gl3.h>           // WebGL2 API
#include <event_dispatch.h>      // Event Handling
#include <event.h>              // Include CanvasUpdateEvent
#include <log.h>

#ifndef ENSURE_MAIN_THREAD_CALL(func, instance, event)
#define ENSURE_MAIN_THREAD_CALL(func, instance, event) \
    if (!emscripten_is_main_browser_thread()) { \
        spdlog::warn("{} is being called from a worker thread! Offloading to main thread.", #func); \
        using EventType = std::remove_reference_t<decltype(event)>; \
        emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_V, &func##<EventType>, instance, event); \
        return; \
    } \
    func(event);
#endif

class Render : public EventNotifierMixIn<Render> {
private:
    GLuint textureID, VAO, VBO;
    int width, height;
    GLuint shaderProgram;

    void draw(const void* pixelData, bool isDirty);
    void commit_to_gpu(const void* pixelBuffer, int w, int h);


public:
    Render(int w, int h);
    ~Render();

    void init_gl();
    static void init_gl_impl(void* v_this);
    template <typename T>
    void handle_event(EventPtr<T> e);
    template <typename T>
    static inline void handle_event_impl(void* v_this, void* v_e);
};