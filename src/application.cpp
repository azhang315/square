// application.cpp
#include <application.h>
#include <emscripten/html5.h>
#include <memory>
#include <event.h>
#include <event_dispatch.h>



#include <canvas.h>
#include <input.h>
#include <net_transport.h>
#include <render.h>

Application::Application()
{
    init();
}

void Application::init()
{
    m_input = std::make_unique<Input>();
    m_net_transport = std::make_unique<NetTransport>();
    m_canvas = std::make_unique<Canvas>();
    m_render = std::make_unique<Render>(Canvas::HEIGHT, Canvas::WIDTH);

    /* Input Events */
    // Input -> Canvas
    add_listener<MouseDownEvent>(m_input.get(), m_canvas.get());

    // Canvas -> Network
    add_listener<CanvasUpdateEvent>(m_canvas.get(), m_net_transport.get());

    // Canvas -> Render
    add_listener<CanvasUpdateEvent>(m_canvas.get(), m_render.get());

    // Network -> Canvas
    add_listener<ServerStateUpdateEvent>(m_net_transport.get(), m_canvas.get());

}

void Application::run()
{
#ifndef FPS
#define FPS 60
#endif
    emscripten_set_main_loop_arg(this->em_process_frame, this, FPS, true);
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
    // app->m_canvas->draw();



    // 4. Send/Receive Network Events - async - no batching for now
    // net_transport.process_network_events();
}