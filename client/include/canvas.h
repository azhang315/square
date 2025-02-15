// canvas.h
#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>

#include <render.h>
#include <event_dispatch.h>
#include <net_transport.h>
#include <log.h>

#ifndef CANVAS_HEIGHT
#define CANVAS_HEIGHT 128
#endif
#ifndef CANVAS_WIDTH
#define CANVAS_WIDTH 128
#endif
constexpr uint32_t COLOR_WHITE = 0xFFFFFFFF;

class Canvas : public EventNotifierMixIn<Canvas> // NetTransport, Render
{
public:
    Canvas();
    ~Canvas() = default;
    static constexpr uint16_t HEIGHT = CANVAS_HEIGHT;
    static constexpr uint16_t WIDTH = CANVAS_WIDTH;

    uint32_t get_pixel(uint16_t x, uint16_t y) const;
    void set_pixel(const uint16_t x, const uint16_t y, const uint32_t c, const uint64_t seq);
    void clear_canvas();

    void handle_event(EventPtr<CanvasServerUpdateEvent> e)
    {
        SLOG("Canvas <- SERVER UPDATE");
        auto &d = e->data;
        notify_listeners(make_event<CanvasUiBatchUpdateEvent>(d.x, d.y, d.color));
    };
    void handle_event(EventPtr<MouseDownEvent> e)
    {
        auto &d = e->data;

        notify_listeners(make_event<CanvasUiUpdateEvent>(d.x, d.y, d.color));

        // TODO: lock
        int next_seq = pixel_sequence[get_index(d.x, d.y)]++;
        // unlock

        notify_listeners(make_event<CanvasLocalUpdateEvent>(d.x, d.y, d.color, next_seq));
    }

    bool is_dirty() const { return render_dirty; };
    void clear_dirty() { render_dirty = false; };
    const uint32_t *get_pixel_buffer() const { return pixel_buffer.data(); };
    // Event
    void batch_update();

    void apply_server_update_batch(const std::vector<CanvasServerUpdateEvent> &updates);

    // Future Work
    void set_stroke(); // h/w accelerate
private:
    static inline uint32_t get_index(int x, int y) { return y * WIDTH + x; }
    uint64_t local_seq_num = 0;
    std::vector<uint32_t> pixel_buffer;                          // index -> RGBA
    std::unordered_map<uint32_t, uint64_t> pixel_sequence;       // index -> seq_num // refactor
    std::vector<CanvasServerUpdateEvent> batched_server_updates; // conflict resolved

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
