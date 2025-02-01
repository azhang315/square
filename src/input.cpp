// input.cpp
#include <include/input.hpp>
#include <iostream>

void Input::init() {
    // Initialize event handling (register with emscripten if needed)
}

void Input::update() {
    while (!m_event_queue.empty()) {
        InputEvent event = m_event_queue.front();
        m_event_queue.pop();

        process_event(event); // Call internal handler
    }
}

bool Input::is_mouse_down() const {
    auto it = m_current_states.find(Codes::MouseDown);
    return it != m_current_states.end() && it->second;
}

bool Input::is_mouse_down_current_frame() const {
    // Track state changes for single-frame detection
    return false; // Implement based on your frame state tracking
}

void Input::queue_event(InputEvent event) {
    m_event_queue.push(event);
}

bool Input::poll_event(InputEvent& out_event) {
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

void Input::process_event(const InputEvent& event) {
    if (event.code == Codes::MouseDown) {
        // Update input state
        m_current_states[Codes::MouseDown] = true;

        // Log the click event
        std::cout << "Mouse clicked at (" << event.x << ", " << event.y << ")\n";

        // Call user-defined callback if set
        if (m_event_callback) {
            m_event_callback(event);
        }

        // Reset mouse state (to handle single-frame detection)
        m_current_states[Codes::MouseDown] = false;
    }
}
