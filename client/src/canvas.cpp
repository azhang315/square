// canvas.cpp
#include <canvas.h>
#include <emscripten.h>
#include <event.h>
#include <emscripten/em_asm.h>

#include <log.h>

#include "canvas.h"

Canvas::Canvas() {
    SLOG("Canvas::init()");

    pixel_buffer = std::vector<uint32_t>(WIDTH * HEIGHT, COLOR_WHITE);
}

void Canvas::set_pixel(uint16_t x, uint16_t y, uint32_t color, uint64_t seq) {
    if (x >= WIDTH || y >= HEIGHT) return;

    uint32_t index = y * WIDTH + x;
    if (seq >= pixel_sequence[index]) {  // Conflict resolution: last-writer-wins
        pixel_buffer[index] = color;
        pixel_sequence[index] = seq;

        notify_listeners(make_event<CanvasUiUpdateEvent>(x, y, color));
    }
}

uint32_t Canvas::get_pixel(uint16_t x, uint16_t y) const {
    if (x >= WIDTH || y >= HEIGHT) return 0;
    return pixel_buffer[y * WIDTH + x];
}

void Canvas::apply_server_update_batch(const std::vector<CanvasServerUpdateEvent>& updates) {
    for (const auto& e : updates) {
        set_pixel(e.x, e.y, e.color, e.seq);  // Apply with conflict resolution
    }
}

// void Canvas::clear() {
//     std::fill(pixel_buffer.begin(), pixel_buffer.end(), 0xFFFFFFFF);
//     pixel_sequence[i.clear();

//     CanvasUIUpdateEvent new_e(x, y, color);
//     notify_listeners(new_e);
// }
