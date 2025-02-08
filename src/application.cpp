// application.cpp
#include <application.h>
#include <emscripten/html5.h>
#include <memory>
#include <event.h>
#include <event_dispatch.h>

#include <factory.h>


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



    // m_input = std::make_unique<Input>();
    // m_net_transport = std::make_unique<NetTransport>();
    // m_canvas = std::make_unique<Canvas>();
    // m_render = std::make_unique<Render>(Canvas::HEIGHT, Canvas::WIDTH);
    Canvas canvas;
    // 1. Create Subsystems with Placeholders for Cyclic Dependencies
    m_net_transport = factory_create_subsystem<NetTransport>(&canvas);  // No listeners yet
    m_render = factory_create_subsystem<Render>();
    auto c = factory_create_subsystem<NetTransport, std::nullptr_t>(nullptr);
    // 2. Canvas with NetTransport and Render as Listeners
    m_canvas = factory_create_subsystem<Canvas>(*m_net_transport, *m_render);

    // 3. Input listens to Canvas
    m_input = factory_create_subsystem<Input>(*m_canvas);

    // 4. Fix Cyclic Dependencies (if needed)
    fixup_subsystem(*m_net_transport, m_canvas.get());
    fixup_subsystem(*m_canvas, m_net_transport.get(), m_render.get());

    /* Input Events */
    // Input -> Canvas
    // add_listener(MouseDownEvent::StaticName(), m_input.get(), m_canvas.get());
    // add_listener<MouseDownEvent, Input, Canvas>(m_input.get(), m_canvas.get());


    // Canvas -> Network
    // add_listener(CanvasUpdateEvent::StaticName(), m_canvas.get(), m_net_transport.get());
    // add_listener<CanvasUpdateEvent>(m_canvas.get(), m_net_transport.get());

    // Canvas -> Render
    // add_listener(CanvasUpdateEvent::StaticName(), m_canvas.get(), m_render.get()); // UI updated - batch diff, set dirty
    // add_listener<CanvasUpdateEvent>(m_canvas.get(), m_render.get());

    // Network -> Canvas
    // add_listener(ServerStateUpdateEvent::StaticName(), m_net_transport.get(), m_canvas.get());
    // add_listener<ServerStateUpdateEvent>(m_net_transport.get(), m_canvas.get());

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