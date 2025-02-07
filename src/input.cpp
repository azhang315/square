// input.cpp
#include <include/input.hpp>
#include <iostream>
#include <emscripten/html5.h>

void Input::init() {
    m_current_states[InputCode::MouseDown] = false;
    emscripten_set_mousedown_callback("#canvas", this, true, on_mouse_down);
    emscripten_set_mouseup_callback("#canvas", this, true, on_mouse_up);
}
static EM_BOOL on_mouse_down(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    Input* input = static_cast<Input*>(userData);
    if (input) {
        Event event(EventType::MouseDown, EventMouseDown{e->clientX, e->clientY});
        input->notify_listeners(event);
    }
    return EM_TRUE;
}
// static EM_BOOL on_mouse_up(int eventType, const EmscriptenMouseEvent* e, void* userData) {
//     Input* input = static_cast<Input*>(userData);
//     if (input) {
//         Event event(EventType::MouseUp, EventMouseUp{e->clientX, e->clientY});
//         input->notify_listeners(event);
//     }
//     return EM_TRUE;
// }

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


// Batched Events
void Input::process_event(const InputEvent& e) {
    switch (event.code) {
        // MouseDown, MouseUp served immediately
        // case InputCode::MouseDown:
        // process_event_mouse_down(event);
        // EventNotifierMixIn::notify_listeners(e_type, e_data); // e.g., call Listeners (canvas, net) functions
        // break;
        // case InputCode::MouseUp:
        // process_event_mouse_up(event);
        // EventNotifierMixIn::notify_listeners(e_type, e_data);
        // break;

        case EventType::NetworkUpdateErr:
    }
}


// static inline void process_event_mouse_down(InputEvent event) {
//     // m_current_states[InputCode::MouseDown] = true;
    
//     canvas_manager->draw_pixel(x, y, color);
//     net_transport->send_mouse_event(x, y, color);
// }
// static inline void process_event_mouse_up(InputEvent event) {
//     m_current_states[InputCode::MouseDown] = false;
// }
