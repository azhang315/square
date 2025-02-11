// canvas.h
#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#include <render.h>
#include <event_dispatch.h>
#include <net_transport.h>

constexpr uint32_t c_white = 0xFFFFFFFF;
// struct PixelUpdate
// {
//     uint16_t x, y;
//     uint32_t color;
//     uint64_t seq; // CRDT: last modified seq
// };

// struct PixelHistory - implementable via simple counter?
// { // Sliding Window
//     // std::deque<int>
//     void ack_update();
//     void add_update();
// };

class Canvas : public EventNotifierMixIn<Canvas> // NetTransport, Render
{
public:
    Canvas();
    ~Canvas() = default;
    static constexpr uint16_t HEIGHT = 256;
    static constexpr uint16_t WIDTH = 256;

    uint32_t get_pixel(uint16_t x, uint16_t y) const;
    void set_pixel(const uint16_t x, const uint16_t y, const uint32_t c, const uint64_t seq);
    void clear_canvas();

    void handle_event(const Event<CanvasServerUpdateEvent> &e) {
        spdlog::info("Canvas <- SERVER UPDATE");

        CanvasUiBatchUpdateEvent new_e1;
        notify_listeners(new_e1);
    };
    void handle_event(const Event<MouseDownEvent> &e)
    {
        spdlog::info("Canvas <- MOUSE DOWN");

        auto mde = static_cast<const MouseDownEvent &>(e);

        CanvasUiUpdateEvent new_e1(mde.x, mde.y, mde.color);
        notify_listeners(new_e1);

        // TODO: lock
        int next_seq = pixel_sequence[get_index(mde.x, mde.y)]++;
        // unlock
        CanvasLocalUpdateEvent new_e2(mde.x, mde.y, mde.color, next_seq);
        notify_listeners(new_e2);
    }

    bool is_dirty() const { return render_dirty; };
    void clear_dirty() { render_dirty = false; };
    const uint32_t *get_pixel_buffer() const { return pixel_buffer.data(); };
    // Event
    void batch_update();

    void apply_server_update_batch(const std::vector<CanvasServerUpdateEvent>& updates);

    // Future Work
    void set_stroke(); // h/w accelerate
private:
    static inline uint32_t get_index(int x, int y) {return y * WIDTH + x;}
    uint64_t local_seq_num = 0;
    std::vector<uint32_t> pixel_buffer;                    // index -> RGBA
    std::unordered_map<uint32_t, uint64_t> pixel_sequence; // index -> seq_num
    std::vector<CanvasServerUpdateEvent> batched_server_updates;              // conflict resolved


    // std::vector<> batched_updates;              // conflict resolved

    // TODO: locality

    // EACH PIXEL: UNACKED SLIDING WINDOW history:
    // * ServerAuth message moves it up
    // @ Pixel: (x,y)
    // seqs:
    // [10 20 30 40 50]
    //       (30) - server auth msg
    //       [30 40 50]
    //                  (60)
    //                  [60]
    // std::unordered_map<std::pair<int, int>, PixelHistory> pixel_history_map;
    // std::unordered_set<std::pair<int, int>> dirty_pixels;

    bool render_dirty;
};
