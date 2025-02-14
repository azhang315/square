// application.cpp
#include <application.h>
#include <emscripten/html5.h>
#include <memory>
#include <event.h>
#include <event_dispatch.h>

#include <emscripten/proxying.h>


#include <canvas.h>
#include <input.h>
#include <net_transport.h>
#include <render.h>

#include <log.h>
#include <emscripten/threading.h> 
#include <emscripten/em_asm.h>
#include <emscripten/eventloop.h>

Application::Application()
{
  SLOG("Application::Application()");
  SLOG("Skip: Logger initialization");
    init();
}

void offload_render_initgl(void* arg) {
  SLOG("offload_render_initgl()");
  std::shared_ptr<Render>* m_render_ptr = static_cast<std::shared_ptr<Render>*>(arg);
  (*m_render_ptr)->init_gl();
} // keptalive

void Application::init()
{
  SLOG("Application::init()");

    m_input = std::make_unique<Input>();
    m_canvas = std::make_unique<Canvas>();
    m_render = std::make_shared<Render>(Canvas::HEIGHT, Canvas::WIDTH);
  
    SLOG("TRY offload_render_initgl()");
    emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VI, offload_render_initgl, &m_render);
    
    // emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_V, [&]() {
    // // m_net_transport = std::make_unique<NetTransport>();
    // });

    SLOG("attempt construction");
    SLOG("success");


    SLOG("Application::init() -> subsystems constructed");

    #ifndef REMOTE_SERVER_IP
    #define REMOTE_SERVER_IP "localhost"
    #define REMOTE_SERVER_PORT 9000
    #endif
    // m_net_transport->start_udp_client(REMOTE_SERVER_IP, REMOTE_SERVER_PORT);

    

    /* Input Events */
    // Input -> Canvas // only one that works
    add_listener<MouseDownEvent>(m_input.get(), m_canvas.get());

    // Canvas -> Network
    // add_listener<CanvasLocalUpdateEvent>(m_canvas.get(), m_net_transport.get());

    // Canvas -> Render
    add_listener<CanvasUiUpdateEvent>(m_canvas.get(), m_render.get());

    // Network -> Canvas
    // add_listener<CanvasServerUpdateEvent>(m_net_transport.get(), m_canvas.get());


}

void Application::run()
{
  SLOG("Application::run()");

    emscripten_set_main_loop_arg(this->em_process_frame, this, 0, true); // rAF
}


void Application::em_process_frame(void *arg)
{
    // SLOG("Application::em_process_frame()");

    Application *app = static_cast<Application *>(arg);

    // 1. Queued Event Processing
    // app->m_input->update(); // poll new inputs

    //            input
    //              |
    //               \
    //          net<->canvas<->render


    // Batched: Network Events
    // Batched: Synchronization Processing

    // 3. Render Frame
    // app->m_canvas->draw();



    // 4. Send/Receive Network Events - async - no batching for now
    // net_transport.process_network_events();
}