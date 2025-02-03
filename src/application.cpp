// application.cpp
#include <include/application.hpp>
#include <emscripten/html5.h>
#include <memory>

Application::Application()
{
    init();
}

void Application::init() {
    m_render = std::make_unique<Render>();
    m_input = std::make_unique<Input>(std::function<void(const InputEvent&)>(handle_input_event));
    m_net_transport = std::make_unique<NetTransport>();
    m_replication = std::make_unique<Replication>();
    m_canvas = std::make_unique<Canvas>();
}
void Application::handle_input_event(const InputEvent& e) {
    switch (e.code) {
        case InputCode::MouseDown:
        process_event_mouse_down(e);
        break;
        case InputCode::MouseUp:
        process_event_mouse_up(e);
        break;
    }
}
inline void process_event_mouse_down(const InputEvent& e) {
    // m_current_states[InputCode::MouseDown] = true;
    m_canvas->draw_pixel(x, y, color);
    m_net_transport->send_mouse_event(x, y, color);
}
inline void Application::process_event_mouse_up(const InputEvent& e) {
    // m_current_states[InputCode::MouseDown] = false;
}


void Application::one_iter() {
    // float delta_time = get_frame_time();
    

    // 1. Input Events (Mouse, network)
    // m_input->update();
    m_input;

    //            input
    //              |
    //             / \
    //          net   canvas

    // 3. Render Frame
    if (render_dirty) {
        renderer.draw();
    }

    // 4. Send/Receive Network Events
    net_transport.process_network_events();



}