// canvas.hpp
#pragma once
#include <vector>
#include <cstdint>

typedef uint16_t pixel_t;
typedef uint32_t color_t;
constexpr color_t c_white = 0xFFFFFFFF;
class Canvas
{
    std::vector<color_t> canvas;
public:
    static constexpr pixel_t HEIGHT = 1000;
    static constexpr pixel_t WIDTH = 1000;
    Canvas();
    void set_pixel(const pixel_t x, const pixel_t y, const color_t c);
    color_t get_pixel(pixel_t x, pixel_t y) const;


    void set_stroke(); // h/w acceleration: call renderer functions
    
};
