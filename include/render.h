#pragma once
#include <GLES3/gl3.h>           // WebGL2 API
#include <event_dispatch.h>      // Event Handling
#include <spdlog/spdlog.h>
#include <event.h>              // Include CanvasUpdateEvent

class Render : public EventNotifierMixIn<Render> {
private:
    GLuint textureID, VAO, VBO;
    int width, height;
    GLuint shaderProgram;

public:
    Render(int w, int h);
    ~Render();

    void init();
    void draw(const void* pixelData, bool isDirty);
    void commit_to_gpu(const void* pixelBuffer, int w, int h);
    void handle_event(const Event<CanvasUpdateEvent>& e);
};