// application.cpp
#include <application.h>
#include <emscripten/html5.h>
#include <memory>
#include <event.h>

Application::Application()
{
    init();
}

void Application::init()
{
    // m_render = std::make_unique<Render>();
    m_input = std::make_unique<Input>();
    m_net_transport = std::make_unique<NetTransport>();
    m_canvas = std::make_unique<Canvas>();

    /* Input Events */
    // Input -> Canvas
    add_listener(MouseDownEvent::StaticName(), m_input.get(), m_canvas.get());

    // Canvas -> Network
    add_listener(CanvasUpdateEvent::StaticName(), m_canvas.get(), m_net_transport.get());

    // Canvas -> Render
    add_listener(CanvasUpdateEvent::StaticName(), m_canvas.get(), m_render.get()); // UI updated - batch diff, set dirty

    // Network -> Canvas
    add_listener(ServerStateUpdateEvent::StaticName(), m_net_transport.get(), m_canvas.get());
}

void Application::run()
{
#ifndef FPS
#define FPS 60
#endif
    emscripten_set_main_loop_arg(this->em_process_frame, this, FPS, true);
}

template <typename Tn, typename Tl>
static inline void add_listener(const char *, Tn *notifier, Tl *listener)
{
    static_assert(std::is_base_of_v<EventNotifierMixIn<Tn>, Tn>,
                  "Notifier must derive from EventNotifierMixIn");
    static_assert(std::is_base_of_v<EventListenerMixIn<Tl>, Tl>,
                  "Listener must derive from EventListenerMixIn");

    notifier->add_listener(e_type, notifier, listener);
}

void Application::em_process_frame(void *arg)
{
    Application *app = static_cast<Application *>(arg);

    // 1. Queued Event Processing
    app->m_input->update(); // poll new inputs

    //            input
    //              |
    //               \
    //          net<->canvas

    // Batched: Network Events
    // Batched: Synchronization Processing

    // 3. Render Frame
    app->m_canvas->draw();

    // 4. Send/Receive Network Events - async - no batching for now
    // net_transport.process_network_events();
}