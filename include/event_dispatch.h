// event_dispatch.h
#pragma once
#include <memory>
#include <event.h>

class IEventListener
{
public:
    // virtual void call_handle_event(const EventType &e_type, void *e_data) = 0;
};

template <typename Derived> // Listener
class EventListenerMixIn : public IEventListener
{
public:
    static_assert(std::is_base_of_v<EventListenerMixIn<Derived>, Derived>, "Derived must inherit from EventListenerMixIn<Derived>");

    // static_assert(std::is_invocable_v<decltype(&Derived::handle_event), Derived, const Event<T> & e>, "Derived class must implement handle_event(const EventType&, void*)");

    template <typename T>
    void call_handle_event(const Event<T> &e)
    {
        static_cast<Derived *>(this)->handle_event(e);
        // this->handle_event(e);
    }
};

template <typename Derived> // Notifier
class EventNotifierMixIn
{
public:
    template <typename ListenerType>
    void add_listener(char *name, ListenerType *l)
    {
        static_assert(std::is_base_of_v<IEventListener, ListenerType>, "Listener class T must derive from IEventListener"); // FIXME:
        static_assert(std::is_base_of_v<EventListenerMixIn<ListenerType>, ListenerType>, "Listener class T must derive from EventListener<T>");

        m_listeners[name].emplace_back(l);
    }

    // void notify_listeners(const EventType &e_type, const EventData)
    template <typename EventType>
    void notify_listeners(const Event<EventType> &e)
    {
        // for (auto *l : m_listeners[e.e_type])
        for (auto *l : m_listeners[e.Static_Name()])
        {
            // l->call_handle_event(e_type, e_data);
            l->call_handle_event(e);
        }
    }

private:
    // std::unordered_map<EventType, std::vector<IEventListener *>> e_listeners;
    std::unordered_map<char *, std::vector<IEventListener *>> m_listeners;
};

/* Alternatives with RTTI, or function indirection */
