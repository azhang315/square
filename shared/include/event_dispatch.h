// event_dispatch.h
#pragma once
#include <memory>
#include <event.h>
#include <unordered_map>
#include <vector>
#include <functional>
#include <utility>
#include <log.h>

template <typename Te>
using EventPtr = std::shared_ptr<const Event<Te>>;

// Default + Specialization
template <typename Tl, typename Te, typename = void>
struct HasHandleEvent : std::false_type
{
};
template <typename Tl, typename Te>
struct HasHandleEvent<Tl, Te, std::void_t<decltype(std::declval<Tl>().handle_event(std::declval<EventPtr<Te>>()))>> : std::true_type
{
};

// Templated
template <typename Tl, typename Te>
constexpr bool HasHandleEvent_v = HasHandleEvent<Tl, Te>::value;

template <typename Te, typename... Args>
inline EventPtr<Te> make_event(Args &&...args)
{
    return std::make_shared<const Event<Te>>(std::forward<Args>(args)...);
}

template <typename Derived> // Notifier
class EventNotifierMixIn
{
public:
    EventNotifierMixIn() = default;
    template <typename Te>
    void add_listener(std::function<void(EventPtr<Te>)> callback)
    {
        m_listeners<Te>.emplace_back(callback);
    }

    template <typename Te>
    void notify_listeners(EventPtr<Te> e)
    {
        auto &l = m_listeners<Te>;
        if (l.size() == 0)
        {
            SLOG("EventNotifier::notify_listeners(): no handlers registered");
        }

        for (auto &callback : m_listeners<Te>)
        {
            callback(e);
        }
    }

private:
    template <typename Te>
    inline static std::vector<std::function<void(EventPtr<Te>)>> m_listeners;
};

// Global add_listener Helper
template <typename Te, typename Tn, typename Tl>
extern inline void add_listener(Tn *notifier, Tl *listener)
{
    static_assert(std::is_base_of_v<EventNotifierMixIn<Tn>, Tn>,
                  "Tn must derive from EventNotifierMixIn<Tn>");
    static_assert(HasHandleEvent_v<Tl, Te>,
                  "Listener must have a handle_event(const Event<EventType>&) function");

    SLOG("Registering Listeners...");
    SLOG("* Notifier -> {}", typeid(Tn).name());
    SLOG("* Listener -> {}", typeid(Tl).name());

    notifier->template add_listener<Te>([listener](EventPtr<Te> event)
                                        { listener->handle_event(event); });
}

/* Alternatives with RTTI, or function indirection */
