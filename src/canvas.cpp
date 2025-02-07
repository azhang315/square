// canvas.cpp
#include "canvas.hpp"
#include <emscripten.h>
#include <types.hpp>
#include <emscripten/em_asm.h>

Canvas::Canvas() : pixel_buffer(HEIGHT * WIDTH, c_white),
                   render(WIDTH, HEIGHT) {}

void Canvas::handle_event(const Event &e)
{
    std::visit([&](auto &&data)
               {
        using T = std::decay_t<decltype(data)>;

        if constexpr (std::is_same_v<T, EventServerStateUpdate>)
            Canvas::apply_server_state_update(data);
        else if constexpr (std::is_same_v<T, EventServerStateConflict>)
            Canvas::apply_server_state_conflict_rollback(data);
        else if constexpr (std::is_same_v<T, EventMouseDown>)
            Canvas::apply_optimistic_local_update(data); }, e.e_data);
}
inline void Canvas::apply_optimistic_local_update(const EventMouseDown &d)
{
    // batch_update();
    // set_pixel()

    // pixel history, add update
    auto& pixel = pixel_history_map[{x, y}];
    pixel.add_update(seq_num, color);
    
    dirty_pixels.insert({x, y});  // Mark pixel as changed (but don't send to GPU yet)

    Event e(EventType::ClientStateUpdate, EventClientStateUpdate{d.x, d.y, local_seq_num});
    notify_listeners(e); // evokes SYNCHRONOUSLY, network thread. Also, if becomes async, need to keep ptrs/ref alive
}
inline void Canvas::apply_server_state_conflict_rollback(const EventServerStateConflict &d) {}
inline void Canvas::apply_server_state_update(const EventServerStateUpdate &d) {
void apply_server_update(uint64_t server_seq, uint32_t server_color, int x, int y) {
    auto& pixel = pixel_history_map[{x, y}];
    
    pixel.ack_update(server_seq, server_color); // Shift window
    
    // If we have newer local updates, reapply them
    if (!pixel.seq_buffer.empty()) {
        uint32_t latest_color = pixel.seq_buffer.back().second;
        apply_color_to_canvas(x, y, latest_color);
    }

    // auto& pixel = pixel_history_map[{x, y}];
    // pixel.ack_update(server_seq, server_color);

    dirty_pixels.insert({x, y});  // Mark as changed (for next GPU commit)
}
}

inline void Canvas::batch_update() {}

extern "C"
{
    EMSCRIPTEN_KEEPALIVE
    void Canvas::clear_canvas()
    {
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
    uint32_t Canvas::get_pixel(uint16_t x, uint16_t y) const
    {
        return (x < WIDTH && y < HEIGHT) ? pixel_buffer[x + y * WIDTH] : c_white;
    }

    EMSCRIPTEN_KEEPALIVE
    const uint32_t *Canvas::get_pixel_buffer() const
    {
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