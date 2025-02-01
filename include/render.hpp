// renderer.hpp
#pragma once

class Render {
public:
    Render() = default;
    ~Render() = default;

    bool init(); // WebGPU
    void clear(); // r g b a

private:
    // WebGPU contexts + handlers
    // Dirty
};