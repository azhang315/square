// renderer.h
#pragma once
#include <GLES3/gl3.h>           // Emscripten WebGL API
#include <event_dispatch.h>      // For EventListenerMixIn, EventNotifierMixIn
#include <spdlog/spdlog.h>


class Render : public EventNotifierMixIn<Render> {
private:
    GLuint textureID;
    int width, height;

public:
    Render(int w, int h);
    ~Render();

    void init();
    
    void draw(const void* pixelData, bool isDirty);
    void commit_to_gpu(const void* pixelBuffer, int w, int h);


    void handle_event(const Event<CanvasUpdateEvent> &e) {
        spdlog::info("Render::Event<CanvasUpdateEvent>()");
    };
};
