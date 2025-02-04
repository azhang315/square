// canvas.cpp
#include "canvas.hpp"
#include <emscripten.h>
#include <types.hpp>
#include <emscripten/em_asm.h>

Canvas::Canvas() : pixel_buffer(HEIGHT * WIDTH, c_white),
render(WIDTH, HEIGHT) {}

void Canvas::handle_event(const EventType& e_type, void* e_data) {
    // auto data = cast(e_type, e_data);
    switch (e_type) {
        case EventType::NetworkServerStateUpdate:
            // validate_server_state_update(data);
            // apply_server_state_update(data); // batch
        case EventType::NetworkServerStateConflict:
            // validate_server_state_conflict(data);
            // apply_server_state_conflict_rollback(data); // batch
    }
}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void Canvas::clear_canvas() {
        std::fill(pixel_buffer.begin(), pixel_buffer.end(), c_white);
    }
    EMSCRIPTEN_KEEPALIVE
    void Canvas::set_pixel(uint16_t x, uint16_t y, uint32_t c)
    {
        if (x < WIDTH && y < HEIGHT)
        {
            pixel_buffer[x + y * WIDTH] = c;
        }
    }
    // void Canvas::set_pixel(int x, int y, uint32_t color) {
    //     int index = (y * width + x) * 4; // RGBA
    //     pixelBuffer[index] = (color >> 24) & 0xFF;
    //     pixelBuffer[index + 1] = (color >> 16) & 0xFF;
    //     pixelBuffer[index + 2] = (color >> 8) & 0xFF;
    //     pixelBuffer[index + 3] = (color) & 0xFF;
    // }

    EMSCRIPTEN_KEEPALIVE
    uint32_t Canvas::get_pixel(uint16_t x, uint16_t y) const {
        return (x < WIDTH && y < HEIGHT) ? pixel_buffer[x + y * WIDTH] : c_white;
    }

    EMSCRIPTEN_KEEPALIVE
    const uint32_t* Canvas::get_pixel_buffer() const {
        return pixel_buffer.data();
    }






// // Dummy network update simulation
// void EMSCRIPTEN_KEEPALIVE processNetworkEvents() {
//     static int counter = 0;
//     int x = counter % CANVAS_WIDTH;
//     int y = (counter / CANVAS_WIDTH) % CANVAS_HEIGHT;
//     uint32_t color = 0xFF0000FF;  // Red color

//     setPixel(x, y, color);
//     counter++;
// }

// // Main loop (Emscripten will call this continuously)
// void main_loop() {
//     processNetworkEvents();
// }

// // Initialize WebAssembly
// void EMSCRIPTEN_KEEPALIVE startMainLoop() {
//     emscripten_set_main_loop(main_loop, 0, 1);
// }
}