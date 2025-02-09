// canvas.cpp
#include <canvas.h>
#include <emscripten.h>
#include <event.h>
#include <emscripten/em_asm.h>

// Canvas::Canvas() : EventNotifierMixIn<Canvas>(),
//                    pixel_buffer(HEIGHT * WIDTH, c_white)
// {
// }
//    pixel_history_map({}),
//    dirty_pixels({})

// void Canvas::handle_event(const Event &e)
// template <typename T>
// void Canvas::handle_event(const Event<T> &e)
// {
//     if constexpr (std::is_same_v<T, ServerStateUpdateEvent>)
//     {
//         apply_server_state_update(e);
//     }
//     else if constexpr (std::is_same_v<T, ServerStateUpdateEvent>)
//     {
//         apply_server_state_conflict_rollback(e);
//     }
//     else if constexpr (std::is_same_v<T, MouseDownEvent>)
//     {
//         apply_optimistic_local_update(e);
//     }
//     else
//     {
//         static_assert(false, "Unhandled event type!");
//     }
// }
inline void Canvas::apply_optimistic_local_update(const MouseDownEvent &e)
{
    // auto& pixel = pixel_history_map[{x, y}];
    // pixel.add_update(color); // embed seqnum

    // dirty_pixels.insert({e.x, e.y});  // Mark pixel as changed (but don't send to GPU yet)
    render_dirty = true;

    CanvasUpdateEvent new_e(e.x, e.y, e.color);
    notify_listeners(new_e); // evokes SYNCHRONOUSLY, network thread. Also, if becomes async, need to keep ptrs/ref alive
}
// inline void Canvas::apply_server_state_conflict_rollback(const ServerStateConflictEvent &e) {}
inline void Canvas::apply_server_state_update(const ServerStateUpdateEvent &e) {}
void apply_server_update(uint64_t server_seq, uint32_t server_color, int x, int y)
{
    // auto& pixel = pixel_history_map[{x, y}];

    // pixel.ack_update(server_seq, server_color); // Shift window

    // // If we have newer local updates, reapply them
    // if (!pixel.seq_buffer.empty()) {
    //     uint32_t latest_color = pixel.seq_buffer.back().second;
    //     apply_color_to_canvas(x, y, latest_color);
    // }

    // // auto& pixel = pixel_history_map[{x, y}];
    // // pixel.ack_update(server_seq, server_color);

    // dirty_pixels.insert({x, y});  // Mark as changed (for next GPU commit)
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

    // EMSCRIPTEN_KEEPALIVE
    // const uint32_t *Canvas::get_pixel_buffer() const
    // {
    //     return pixel_buffer.data();
    // }

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