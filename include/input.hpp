// input_manager.hpp
#pragma once
#include <unordered_map>
#include <queue>
#include <functional>

enum struct Codes {
    MouseDown
};
struct InputEvent {
    Codes code;
    int x, y;
};
class Input {
public:
    using EventCallback = std::function<void(const InputEvent&)>;
    
    Input() = default;
    ~Input() = default;

    void init(); // register with emscripten
    void update();

    bool is_mouse_down() const;
    bool is_mouse_down_current_frame() const;

    void queue_event(InputEvent event);
    void poll_event(InputEvent& out);
    
    void set_event_callback(EventCallback callback);
private:
    void process_event(const InputEvent& event);
    std::unordered_map<Codes, bool> m_current_states;
    std::queue<InputEvent> m_event_queue;
    EventCallback m_event_callback;
};