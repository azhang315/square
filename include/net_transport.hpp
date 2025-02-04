// net_transport.hpp
#pragma once
#include <cstddef>
#include "event_notification.hpp"

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
    void handle_event(const EventType& event_type, void* e_data);

// Mix-in inherited
protected: 
    /* Notifier Interface (Publishes to Listeners) */
    void notify_listeners(const EventType& event_type, void* e_data); 

    /* Example usage:
    void on_event(network recv) {
        notify_listeners(EventType{}, 0);
    }
    */
};