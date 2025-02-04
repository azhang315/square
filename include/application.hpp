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

    void one_iter(); // main
    // Events processing
    // ...
private:
    void init();
    std::unique_ptr<Render> m_render;
    std::unique_ptr<Input> m_input;
    std::unique_ptr<NetTransport> m_net_transport;
    std::unique_ptr<Replication> m_replication;
    std::unique_ptr<Canvas> m_canvas;
};