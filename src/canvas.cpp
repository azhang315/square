// canvas.cpp
#include "canvas.hpp"
#include <emscripten.h>

Canvas::Canvas() : canvas(HEIGHT * WIDTH, c_white) {}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void Canvas::set_pixel(pixel_t x, pixel_t y, color_t c)
    {
        if (x < WIDTH && y < HEIGHT)
        {
            canvas[x + y * WIDTH] = c;
        }
    }
    color_t Canvas::get_pixel(pixel_t x, pixel_t y) const
    {
        return (x < WIDTH && y < HEIGHT) ? canvas[x + y * WIDTH] : c_white;
    }
}