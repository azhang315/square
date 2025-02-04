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

    // Input events
    add_listener(EventType::MouseDown, m_input.get(), m_net_transport.get());
    add_listener(EventType::MouseDown, m_input.get(), m_canvas.get());

    // Render events
    add_listener(EventType::CanvasUpdated, m_canvas.get(), m_render.get()); // UI updated - batch diff, set dirty
}


template <typename Tn, typename Tl>
static inline void add_listener(EventType e_type, Tn* notifier, Tl* listener) {
    static_assert(std::is_base_of_v<EventNotifierMixIn<Tn>, Tn>, 
                  "Notifier must derive from EventNotifierMixIn");
    static_assert(std::is_base_of_v<EventListenerMixIn<Tl>, Tl>, 
                  "Listener must derive from EventListenerMixIn");
    
    notifier->add_listener(e_type, notifier, listener);
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