#include <render.hpp>
#include <GLES3/gl3.h> // Emscripten WebGL API
#include <canvas.hpp>

Render::Render(int w, int h) : width(w), height(h), textureID(0) {}

Render::~Render() {
    glDeleteTextures(1, &textureID);
}

void Render::init() {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Allocate empty texture storage
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void Render::draw(Canvas& canvas) {
    if (!canvas.is_dirty()) return;

    canvas.apply_batch_update();  // Update pixel buffer

    glBindTexture(GL_TEXTURE_2D, textureID);  // Bind the texture

    // Update texture with new pixel data
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, canvas.get_pixel_buffer());

    canvas.clear_dirty();  // Mark canvas as clean
}
