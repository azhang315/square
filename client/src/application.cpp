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

#include <log.h>

Application::Application()
{
  spdlog::info("Application::Application()");
  spdlog::info("Skip: Logger initialization");
    init();
}

void Application::init()
{
  spdlog::info("Application::init()");

    m_input = std::make_unique<Input>();
    m_net_transport = std::make_unique<NetTransport>();
    m_canvas = std::make_unique<Canvas>();
    m_render = std::make_unique<Render>(Canvas::HEIGHT, Canvas::WIDTH);

    spdlog::info("Application::init() -> unique pointers made");



    /* Input Events */
    // Input -> Canvas // only one that works
    add_listener<MouseDownEvent>(m_input.get(), m_canvas.get());

    // Canvas -> Network
    add_listener<CanvasLocalUpdateEvent>(m_canvas.get(), m_net_transport.get());

    // Canvas -> Render
    add_listener<CanvasUiUpdateEvent>(m_canvas.get(), m_render.get());

    // Network -> Canvas
    add_listener<CanvasServerUpdateEvent>(m_net_transport.get(), m_canvas.get());


}

void Application::run()
{
  spdlog::info("Application::run()");

    emscripten_set_main_loop_arg(this->em_process_frame, this, 0, true); // rAF
}


void Application::em_process_frame(void *arg)
{
    // spdlog::info("Application::em_process_frame()");

    Application *app = static_cast<Application *>(arg);

    // 1. Queued Event Processing
    app->m_input->update(); // poll new inputs

    //            input
    //              |
    //               \
    //          net<->canvas<->render

                      //     [ Authoritative Server ]
                                //     ↑
                                //     ↓ (Sync: Pixel Updates, Conflict Resolutions)
// [ Input (Mouse/Drag) ] --> [ Canvas (Logical State) ] --> [ Render (WebGL/Scaling) ]


    // Batched: Network Events
    // Batched: Synchronization Processing

    // 3. Render Frame
    // app->m_canvas->draw();



    // 4. Send/Receive Network Events - async - no batching for now
    // net_transport.process_network_events();
}