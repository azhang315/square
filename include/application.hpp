// application.hpp
#pragma once
#include <memory>
#include <include/render.hpp>
#include <include/input.hpp>
#include <include/net_transport.hpp>
#include <include/replication.hpp>
#include <include/canvas.hpp>

class Application {
public:
    Application();
    ~Application();

    // initialize subsystems
    void init();
    static void one_iter(); // main

    static void handle_input_event(const InputEvent& event); // event callback

    // Events processing
    // static void process_event_mouse_down(const InputEvent& event);
    // ...
private:
    std::unique_ptr<Render> m_render;
    std::unique_ptr<Input> m_input;
    std::unique_ptr<NetTransport> m_net_transport;
    std::unique_ptr<Replication> m_replication;
    std::unique_ptr<Canvas> m_canvas;
};