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
struct PixelUpdate
{
    uint16_t x, y;
    uint32_t color;
    uint64_t seq; // CRDT: last modified seq
};

struct PixelHistory
{ // Sliding Window
    // std::deque<int>
    void ack_update();
    void add_update();
};

template <typename... Tl> // Listeners
class Canvas : public EventListenerMixIn<Canvas>,
               public EventNotifierMixIn<Canvas<Tl...>, Tl...> // NetTransport, Render
{
private:
    void handle_event(const Event<MouseDownEvent>& event) {
        notify(event);
    }
public:
    Canvas(Tl &...listeners) : EventNotifierMixIn<Canvas<... Tl>, Tl...>();
    static constexpr uint16_t HEIGHT = 1000;
    static constexpr uint16_t WIDTH = 1000;
    Canvas();
    // Canvas(Render render);

    uint32_t get_pixel(uint16_t x, uint16_t y) const;
    void set_pixel(const uint16_t x, const uint16_t y, const uint32_t c);
    void clear_canvas();

    // void handle_event(const EventType &event_type, void *e_data);
    template <typename T>
    void handle_event(const Event<T> &e);

    // Rendering
    // void draw()
    // {
    //     this->render.draw(*this);
    // }
    bool is_dirty() const { return render_dirty; };
    void clear_dirty() { render_dirty = false; };
    const uint32_t *get_pixel_buffer() const { return pixel_buffer.data(); };
    // Event
    void batch_update();
    void apply_batch_update();

    // Future Work
    void set_stroke(); // h/w accelerate

    // Mix-in inherited
protected:
    /* Notifier Interface (Publishes to Listeners) */
    template <typename T>
    void notify_listeners(const Event<T> &e);

private:
    uint64_t local_seq_num = 0;
    std::vector<uint32_t> pixel_buffer;
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
    std::vector<PixelUpdate> batched_updates;              // conflict resolved // TODO: Locality
    std::unordered_map<uint32_t, uint64_t> pixel_sequence; // index, seq_num // TODO: locality

    // Render render;

    // inline void apply_optimistic_local_update(const EventMouseDown &d);
    // inline void apply_server_state_conflict_rollback(const EventServerStateConflict &d);
    // inline void apply_server_state_update(const EventServerStateUpdate &d);

    inline void apply_optimistic_local_update(const MouseDownEvent &d);
    // inline void apply_server_state_conflict_rollback(const ServerStateConflictEvent &d);
    inline void apply_server_state_update(const ServerStateUpdateEvent &d);
};
