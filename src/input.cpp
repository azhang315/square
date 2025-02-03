// input.cpp
#include <include/input.hpp>
#include <iostream>
#include <emscripten/html5.h>

Input::Input(EventCallback f) {
    set_event_callback(f);
}
void Input::init() {
    m_current_states[InputCode::MouseDown] = false;
    emscripten_set_mousedown_callback("#canvas", this, true, on_mouse_down);
    emscripten_set_mouseup_callback("#canvas", this, true, on_mouse_up);
}
static EM_BOOL on_mouse_down(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    process_event_mouse_down(InputEvent{InputCode::MouseDown, e->clientX, e->clientY});
    return EM_TRUE;
}
static EM_BOOL on_mouse_up(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    process_event_mouse_up(InputEvent{InputCode::MouseUp, e->clientX, e->clientY});
    return EM_TRUE
}

// Process queued input events
void Input::update() {
    while (!m_event_queue.empty()) {
        InputEvent event = m_event_queue.front();
        m_event_queue.pop();

        process_event(event); // Call internal handler
    }
}

inline void Input::queue_one_event(InputEvent event) {
    m_event_queue.push(event); // network
}

bool Input::is_mouse_down() const {
    auto it = m_current_states.find(InputCode::MouseDown);
    return it != m_current_states.end() && it->second;
}

bool Input::is_mouse_down_current_frame() const {
    // Track state changes for single-frame detection
    return false; // Implement based on your frame state tracking
}

bool Input::poll_one_event(InputEvent& out_event) {
    if (!m_event_queue.empty()) {
        out_event = m_event_queue.front();
        m_event_queue.pop();
        return true;
    }
    return false;
}

void Input::set_event_callback(EventCallback callback) {
    m_event_callback = callback;
}

// void Input::process_event(const InputEvent& event) {
//     switch (event.code) {
//         case InputCode::MouseDown:
//         process_event_mouse_down(event);
//         break;
//         case InputCode::MouseUp:
//         process_event_mouse_up(event);
//         break;
//     }

// }
// inline void Input::process_event_mouse_down(InputEvent event) {
//     m_current_states[InputCode::MouseDown] = true;
//     canvas_manager->draw_pixel(x, y, color);
//     net_transport->send_mouse_event(x, y, color);
// }
// inline void Input::process_event_mouse_up(InputEvent event) {
//     m_current_states[InputCode::MouseDown] = false;
// }
