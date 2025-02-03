#pragma once
#include <memory>

template <typename T>
class EventNotifierMixIn {
public:
    void notify_listeners(const ) {
        static_cast<T*>(this)->handle
    }    
};


// };


template <typename Derived>
class EventNotifierMixin {
public:
    void notify_listeners(const std::string& event_type, int x, int y, uint32_t color) {
        static_cast<Derived*>(this)->handle_event(event_type, x, y, color);
    }
};

class NetTransport : public EventNotifierMixin<NetTransport> {
public:
    void handle_event(const std::string& event_type, int x, int y, uint32_t color) {
    }
};