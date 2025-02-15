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
#include <emscripten/posix_socket.h>

Application::Application()
{
  SLOG("Application::Application()");
  init();
}

void Application::init()
{
  SLOG("Application::init()");
  m_input = std::make_unique<Input>();
  m_canvas = std::make_unique<Canvas>();
  m_render = std::make_shared<Render>(Canvas::HEIGHT, Canvas::WIDTH);
  m_net_transport = std::make_unique<NetTransport>();
  SLOG("Application::init() - subsystems constructed");
#ifndef REMOTE_SERVER_IP
#endif
  constexpr char *remote_server_ip = "127.0.0.1";
  constexpr int remote_server_port = 9000;
  m_net_transport->start_udp_client(remote_server_ip, remote_server_port);

  // TODO: add barrier

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
  SLOG("Application::run()");
  emscripten_set_main_loop_arg(this->em_process_frame, this, 0, true); // rAF
}

void Application::em_process_frame(void *arg)
{
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
  // 4. Send/Receive Network Events - async - none
  // net_transport.process_network_events();
}