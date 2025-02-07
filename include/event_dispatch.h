// event_notification.hpp
#pragma once
#include <memory>
#include "event.h"

class IEventListener
{
public:
    // virtual void call_handle_event(const EventType &e_type, void *e_data) = 0;
};

template <typename Derived> // Listener
class EventListenerMixIn : public IEventListener
{
public:
    static_assert(std::is_base_of_v<EventListenerMixIn<Derived>, Derived>,
                  "Derived must inherit from EventListenerMixIn<Derived>");
    static_assert(std::is_invocable_v<decltype(&Derived::handle_event), Derived, const Event e>,
                  "Derived class must implement handle_event(const EventType&, void*)");

    void call_handle_event(const Event e)
    {
        static_cast<Derived *>(this)->handle_event(event);
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

    // void notify_listeners(const EventType &e_type, const EventData)
    void notify_listeners(const Event &e)
    {
        for (auto *l : e_listeners[e.e_type])
        {
            // l->call_handle_event(e_type, e_data);
            l->call_handle_event(e);
        }
    }

private:
    std::unordered_map<EventType, std::vector<IEventListener *>> e_listeners;
};

/* Alternatives with RTTI, or function indirection */
