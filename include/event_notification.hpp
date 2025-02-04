// event_notification.hpp
#pragma once
#include <memory>
#include "types.hpp"

class IEventListener
{
public:
    virtual void call_handle_event(const EventType &e_type, void *e_data) = 0;
};

template <typename Derived> // Listener
class EventListenerMixIn : public IEventListener
{
public:
    static_assert(std::is_base_of_v<EventListenerMixIn<Derived>, Derived>,
                  "Derived must inherit from EventListenerMixIn<Derived>");
    static_assert(std::is_invocable_v<decltype(&Derived::handle_event), Derived, const EventType &, void *>,
                  "Derived class must implement handle_event(const EventType&, void*)");
    void call_handle_event(const EventType &e_type, void *e_data) override
    {
        static_cast<Derived *>(this)->handle_event(e_type, e_data);
    }
};

template <typename Derived> // Notifier
class EventNotifierMixIn
{
public:
    template <typename T>
    void add_listener(const EventType &e_type, T *l)
    {
        static_assert(std::is_base_of_v<IEventListener, T>, "Listener class T must derive from IEventListener");
        static_assert(std::is_base_of_v<EventListenerMixIn<T>, T>, "Listener class T must derive from EventListener<T>");

        e_listeners[e_type].emplace_back(l);
    }

protected:
    void notify_listeners(const EventType &e_type, void *e_data)
    {
        for (auto *l : e_listeners[e_type])
        {
            l->call_handle_event(e_type, e_data);
        }
    }

private:
    std::unordered_map < EventType, std::vector<IEventListener *> e_listeners;
    // TODO: safe pointer
};

/* Alternatives exist with RTTI, or function indirection */

// template <typename Derived> // Notifier
// class EventNotifierMixIn {
// public:
//     using ListenerCallback = std::function<void(const EventType&, void*)>;

//     void add_listener(const EventType& e_type, ListenerCallback c) {
//         listener_callbacks[e_type].push_back(std::move(c));
//     }

// protected:
//     void notify_listeners(const EventType& e_type, void* e_data) {
//         for (auto& c : listener_callbacks[e_type]) {
//             c(e_type, e_data);
//         }
//     }
// private:
//     std::unordered_map<EventType, ListenerCallback> listener_callbacks;
// };