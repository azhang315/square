// net_transport.hpp
#pragma once
#include <cstddef>
#include "event_notifier.hpp"
#include "event_listener.hpp"

class NetTransport : public EventListenerMixIn<NetTransport>,
                     public EventNotifierMixIn<NetTransport>
{
public:
    NetTransport();
    ~NetTransport();

    void sendPacket(const char *data, size_t size);
    void receivePacket();

    void on_server_update(); // get pixel, rollback if incorrect on local
    // may need canvas friendliness, or delegate to app
    // or queue to input / event handler, let event handler manage
    // m_event.queue(net_event)

    // Listener Interface
private:
    // std::vector<NetListener*> m_listeners;
};