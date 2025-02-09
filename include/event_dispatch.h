// event_dispatch.h
#pragma once
#include <memory>
#include <event.h>
#include <unordered_map>
#include <vector>
#include <functional>


// Default + Specialization
template <typename Listener, typename EventType, typename = void>
struct HasHandleEvent : std::false_type {};
template <typename Listener, typename EventType>
struct HasHandleEvent<Listener, EventType, std::void_t<
    decltype(std::declval<Listener>().handle_event(std::declval<Event<EventType>>()))
>> : std::true_type {};
// Template
template <typename Listener, typename EventType>
constexpr bool HasHandleEvent_v = HasHandleEvent<Listener, EventType>::value;


template <typename Derived> // Notifier
class EventNotifierMixIn
{
public:
    EventNotifierMixIn() = default;
    // template <typename Te, typename Tl>
    template <typename Te>
    void add_listener(std::function<void(const Event<Te> &)> callback)
    {
        m_listeners<Te>().emplace_back(callback);
    }

    template <typename Te>
    void notify_listeners(const Event<Te> &e)
    {
        for (auto &callback : m_listeners<Te>())
        {
            callback(e);
        }
    }

private:
    template <typename Te>
    inline std::vector<std::function<void(const Event<Te> &)>> &m_listeners()
    {
        static std::vector<std::function<void(const Event<Te> &)>> l;
        return l;
    }
};
// Global add_listener Helper
template <typename Te, typename Tn, typename Tl>
extern inline void add_listener(Tn *notifier, Tl *listener)
{
    static_assert(std::is_base_of_v<EventNotifierMixIn<Tn>, Tn>,
                  "Tn must derive from EventNotifierMixIn<Tn>");
    static_assert(HasHandleEvent_v<Tl, Te>,
                  "Listener must have a handle_event(const Event<EventType>&) function");

    // add event callback
    notifier->template add_listener<Te>([listener](const Event<Te> &event)
                                        { listener->handle_event(event); });
}

/* Alternatives with RTTI, or function indirection */
