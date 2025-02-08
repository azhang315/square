// renderer.cpp
#include "render.h"
#include <iostream>
#include <GLES3/gl3.h>

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

void Render::draw(const void* pixelBuffer, bool isDirty) {
    if (!isDirty) return;  // Skip if canvas is not dirty

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
}

void Render::commit_to_gpu(const void* pixelBuffer, int w, int h) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

    std::cout << "Committed " << w << "x" << h << " pixels to GPU." << std::endl;
}
