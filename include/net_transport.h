// net_transport.h
#pragma once
#include <cstddef>
#include <event_dispatch.h>

class NetTransport : public EventListenerMixIn<NetTransport>,
                     public EventNotifierMixIn<NetTransport>
{
private:
    void sendPacket(const char *data, size_t size);
    void receivePacket();
public:
    NetTransport();
    ~NetTransport();

    /* Servers */
    void start_client();
    void start_server();




    /* Events  */
    void on_server_update(); // get pixel, rollback if incorrect on local

    /* Listener Interface (Triggered by External Notifier) */
    template <typename T>
    void handle_event(const Event<T>& e);

// Mix-in inherited
protected: 
    /* Notifier Interface (Publishes to Listeners) */
    template <typename T>
    void notify_listeners(const Event<T>& e); 
};