// canvas.hpp
#pragma once
#include <vector>
#include <cstdint>
#include <render.hpp>

constexpr uint32_t c_white = 0xFFFFFFFF;
struct PixelUpdate
{
    uint16_t x, y;
    uint32_t color;
    uint64_t timestamp; // CRDT: Last-modified timestamp
};
class Canvas : EventListenerMixIn<Canvas>,
               EventNotifierMixIn<Canvas>
{
private:
    uint64_t local_seq_num = 0;
    std::vector<uint32_t> pixel_buffer;
    bool render_dirty;
    std::vector<PixelUpdate> batched_updates;              // conflict resolved // TODO: Locality
    std::unordered_map<uint32_t, uint64_t> pixel_sequence; // index, seq_num // TODO: locality

    Render render;
public:
    static constexpr uint16_t HEIGHT = 1000;
    static constexpr uint16_t WIDTH = 1000;
    Canvas();
    // Canvas(Render render);

    uint32_t get_pixel(uint16_t x, uint16_t y) const;
    void set_pixel(const uint16_t x, const uint16_t y, const uint32_t c);
    void clear_canvas();


    void handle_event(const EventType &event_type, void *e_data);

    // Rendering
    void draw()
    {
        this->render.draw(*this);
    }
    bool is_dirty() const { return render_dirty; };
    void clear_dirty() { render_dirty = false; };
    const uint32_t *get_pixel_buffer() const { return pixel_buffer.data(); };
    // Event
    void batch_update();
    void apply_batch_update();

    // Future Work
    void set_stroke(); // h/w accelerate
};
