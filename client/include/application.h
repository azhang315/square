// application.h
#pragma once
#include <memory>

#include <render.h>
#include <input.h>
#include <net_transport.h>
#include <canvas.h>
#include <log.h>

class Application {
public:
    Application();
    ~Application() = default;

    void run();
private:
    void init();
    static void em_process_frame(void* arg);
    void create_render_instance();

    std::shared_ptr<Render> m_render;
    std::unique_ptr<Input> m_input;
    std::unique_ptr<NetTransport> m_net_transport;
    std::unique_ptr<Canvas> m_canvas;
};