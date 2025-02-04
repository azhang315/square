// canvas.hpp
#pragma once
#include <vector>
#include <cstdint>

constexpr uint32_t c_white = 0xFFFFFFFF;
class Canvas
{
private:
    std::vector<uint32_t> pixel_buffer;
public:
    static constexpr uint16_t HEIGHT = 1000;
    static constexpr uint16_t WIDTH = 1000;
    Canvas();
    void clear_canvas();
    void set_pixel(const uint16_t x, const uint16_t y, const uint32_t c);
    uint32_t get_pixel(uint16_t x, uint16_t y) const;
    const uint32_t* get_pixel_buffer() const;

    void set_stroke(); // h/w accelerate
    
};
