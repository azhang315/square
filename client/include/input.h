// input_manager.h
#pragma once
#include <unordered_map>
#include <queue>
#include <functional>
#include <emscripten/em_types.h>
#include <emscripten/html5.h>
// #include "include/types.hpp"
// #include "include/event_notification.hpp"
#include <event.h>
#include <event_dispatch.h>

class Input : public EventNotifierMixIn<Input>
{
public:
    Input();
    ~Input() = default;

    void update() {} // process queued input events

    static EM_BOOL on_mouse_down(int eventType, const EmscriptenMouseEvent *e, void *userData);
    static EM_BOOL on_mouse_up(int eventType, const EmscriptenMouseEvent *e, void *userData);
    static EM_BOOL on_mouse_move(int eventType, const EmscriptenMouseEvent *e, void *userData);

private:
    bool m_mouse_down;
    // inline void queue_one_event(InputEvent event);
    // void process_event(const InputEvent &event);
    // inline void process_event_mouse_down(InputEvent event);
    // inline void process_event_mouse_up(InputEvent event);

    // std::unordered_map<InputCode, bool> m_current_states;
    // std::queue<InputEvent> m_event_queue;

};