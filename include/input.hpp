// input_manager.hpp
#pragma once
#include <unordered_map>
#include <queue>
#include <functional>
#include <emscripten/em_types.h>
#include "include/types.hpp"
#include "include/event_notification.hpp"

class Input : public EventNotifierMixIn<Input>,
              public EventListenerMixIn<Input>
{
public:
    Input() = default;
    ~Input() = default;

    void init();   // register with emscripten
    void update(); // process queued input events

    bool is_mouse_down() const { return m_current_states.at(InputCode::MouseDown); }
    bool is_mouse_down_current_frame() const { return m_mouse_down_current_frame; }

    bool poll_one_event(InputEvent &out);

    static EM_BOOL on_mouse_down(int eventType, const EmscriptenMouseEvent *e, void *userData);
    static EM_BOOL on_mouse_up(int eventType, const EmscriptenMouseEvent *e, void *userData);

private:
    inline void queue_one_event(InputEvent event);
    void process_event(const InputEvent &event);
    inline void process_event_mouse_down(InputEvent event);
    inline void process_event_mouse_up(InputEvent event);

    std::unordered_map<InputCode, bool> m_current_states;
    std::queue<InputEvent> m_event_queue;

    bool m_mouse_down_current_frame = false;
};