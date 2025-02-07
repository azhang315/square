// renderer.h
#pragma once
#include <GLES3/gl3.h> // Emscripten WebGL API
#include <canvas.h>

// template <typename Derived>
// class RenderMixIn {
// public:
//     void call_draw(const Derived* d) {
//         static_assert(std::is_base_of_v<RenderMixIn<Derived>, Derived>, "RenderMixIn<T> must be base of T")
//         static_cast<Derived*>(this)->draw();
//     }
// };

class Render
{
private:
    GLuint textureID;
    int width, height;
public:
    Render(int w, int h);
    ~Render();
    void init();
    void draw(Canvas &canvas);

};