// event_dispatch.h
#pragma once
#include <memory>
#include <event.h>
#include <unordered_map>
#include <vector>


// Compile-time check for event handling
template <typename Listener, typename EventType, typename = void>
struct CanHandleEvent : std::false_type {};

// Specialization if Listener has handle_event(Event<EventType>)
template <typename Listener, typename EventType>
struct CanHandleEvent<Listener, EventType, std::void_t<
    decltype(std::declval<Listener>().handle_event(std::declval<Event<EventType>>()))
>> : std::true_type {};

// Convenience variable template
template <typename Listener, typename EventType>
constexpr bool CanHandleEvent_v = CanHandleEvent<Listener, EventType>::value;



template <typename Derived> // Listener
class EventListenerMixIn
{
public:
    EventListenerMixIn() = default;
    // static_assert(std::is_base_of_v<EventListenerMixIn<Derived>, Derived>, "Derived must inherit from EventListenerMixIn<Derived>");

    template <typename Te>
    void call_handle_event(const Event<Te> &e)
    {
        static_cast<Derived *>(this)->handle_event(e);
    }
};

template <typename Derived, typename... Listeners>
class EventNotifierMixIn {
public:
    EventNotifierMixIn(Listeners&... listeners) : listeners_(listeners...) {}

    template <typename EventType>
    void notify(const Event<EventType>& event) {
        static_cast<Derived*>(this)->template notify_impl<EventType>(event, std::make_index_sequence<sizeof...(Listeners)>{});
    }

private:
    std::tuple<Listeners&...> listeners_;

    template <typename EventType, std::size_t... Indices>
    void notify_impl(const Event<EventType>& event, std::index_sequence<Indices...>) {
        // (std::get<Indices>(listeners_).call_handle_event(event), ...); // unrolled loop at compile
        // stratify by event handle
        (dispatch_if_handled(std::get<Indices>(listeners_), event), ...);
        
    }
    template <typename Listener, typename EventType>
    inline void dispatch_if_handled(Listener& listener, const Event<EventType>& event) {
        if constexpr (CanHandleEvent_v<Listener, EventType>) {
            listener.call_handle_event(event);
        }
    }

    template <typename... NewListeners>
    void fixup(NewListeners*... new_listeners) {
        listeners_ = std::make_tuple(new_listeners...);
    }
};



/*
template <typename Derived> // Notifier
class EventNotifierMixIn
{
public:
    EventNotifierMixIn() = default;
    template <typename Te, typename Tl>
    void add_listener(Tl *l)
    {
        // static_assert(std::is_base_of_v<IEventListener, ListenerType>, "Listener class T must derive from IEventListener"); // FIXME:
        // static_assert(std::is_base_of_v<EventListenerMixIn<ListenerType>, ListenerType>, "Listener class T must derive from EventListener<T>");

        m_listeners<Te>().emplace_back(static_cast<EventListenerMixIn<Tl>*>(l));
    }

    template <typename Te>
    void notify_listeners(const Event<Te> &e)
    {
        for (auto *l : m_listeners<Te>())
        {
            l->call_handle_event(e);
        }   
    }

private:
    // std::unordered_map<std::string, std::vector<IEventListener *>> m_listeners;

    // wtf...
    template <typename Te>
    std::vector<EventListenerMixIn<Derived>*>& m_listeners() {
        static std::vector<EventListenerMixIn<Derived>*> listeners;
        return listeners;
    }
};

*/

/* Alternatives with RTTI, or function indirection */

// fixme: add function pointer callbacks
// // Global add_listener Helper
// template <typename Te, typename Tn, typename Tl>
// extern inline void add_listener(Tn* notifier, Tl* listener) {
    
//     static_assert(std::is_base_of_v<EventNotifierMixIn<Tn>, Tn>,
//                   "Notifier must derive from EventNotifierMixIn");
//     static_assert(std::is_base_of_v<EventListenerMixIn<Tl>, Tl>,
//                   "Listener must derive from EventListenerMixIn");

//     notifier->template add_listener<Te>(listener);
// }
