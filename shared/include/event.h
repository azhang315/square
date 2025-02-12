// event.h
#pragma once
#include <cstdint>


template<typename Derived>
class Event
{
private:
    // EventData m_data; // offload to derived
    
public:
    static const char* StaticName() { return Derived::Name; };
    // Event(Derived type) : m_type(type) {}
    ~Event() = default;

    // inline const Derived GetType() const { return m_Type; }

    // template<typename EventType>
    // inline EventType ToType() const
    // {
    //     return static_cast<const EventType&>(*this);
    // }
};

struct MouseDownEvent : public Event<MouseDownEvent> {
    static constexpr const char* Name = "MouseDownEvent";
    int x;
    int y;
    uint32_t color;
    MouseDownEvent(int x, int y, uint32_t color) : x(x), y(y), color(color) {};
};
struct CanvasUiUpdateEvent : public Event<CanvasUiUpdateEvent> {
    static constexpr const char* Name = "CanvasUiUpdateEvent";
    int x, y;
    uint32_t color;
    CanvasUiUpdateEvent(int x, int y, uint32_t color)
        : x(x), y(y), color(color) {}
};
struct CanvasUiBatchUpdateEvent : public Event<CanvasUiBatchUpdateEvent> {
    static constexpr const char* Name = "CanvasUIBatchUpdateEvent";
    int x, y;
    uint32_t color;
    CanvasUiBatchUpdateEvent(int x, int y, uint32_t color) : x(x), y(y), color(color) {}
};
struct CanvasLocalUpdateEvent : public Event<CanvasLocalUpdateEvent> {
    static constexpr const char* Name = "CanvasLocalUpdateEvent";
    int x, y;
    uint32_t color;
    int seq;
    CanvasLocalUpdateEvent(int x, int y, uint32_t color, int seq) : x(x), y(y), color(color), seq(seq) {}
};

struct CanvasServerUpdateEvent : public Event<CanvasServerUpdateEvent> {
    static constexpr const char* Name = "CanvasServerUpdateEvent";
    int x, y;
    uint32_t color;
    int seq;

    CanvasServerUpdateEvent(int x, int y, uint32_t color, int seq)
        : x(x), y(y), color(color), seq(seq) {}
};


/*

using EventData = std::variant<
    std::monostate,
    EventMouseDown,
    EventServerStateConflict,
    EventServerStateUpdate,
    EventCanvasUpdate,
    EventClientStateUpdate>;


enum struct EventType
{
    // Network Events
    // NetworkPacketRecv,
    // NetworkUpdateErr, // Rollback / sync error
    ServerStateUpdate,   // authoritative Update
    ServerStateConflict, // authoritative Update
                         // ServerStateUpdate, // Authoritative Update

    ClientStateUpdate,

    // Input Events
    MouseDown,
    // MouseUp,

    // Rendering
    CanvasUpdate, // Pixel update applied
                  // FrameRendered,
                  // RedrawRequest,

    // Replication & State Sync
    // StateSyncStart,          // State synchronization process begins
    // StateSyncComplete,       // State synchronization process ends
    // ReplicationPacketSent,   // Replication packet was sent
    // ReplicationPacketReceived, // Replication packet was received

    // // Application Lifecycle
    // AppInitialized,          // Application finished initialization
    // ShutdownRequested,       // Application received shutdown signal
    // FatalError               // Unrecoverable application error

};*/