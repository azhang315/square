#pragma once
#include <variant>

struct EventServerStateUpdate
{
};
struct EventServerStateConflict
{
};
struct EventCanvasUpdate
{
};
struct EventClientStateUpdate
{
    int32_t x;
    int32_t y;
    uint64_t seq;
};
struct EventMouseDown
{
    int32_t x;
    int32_t y;
};



template<typename Derived>
class Event
{
private:
    // EventData m_data; // offload to derived
public:
    static const char* StaticName() { return Derived::Name; };
    Event() = delete;
    Event(Derived type) : m_type(type) {}
    ~Event() = default;

    inline const Derived GetType() const { return m_Type; }

    template<typename EventType>
    inline EventType ToType() const
    {
        return static_cast<const EventType&>(*this);
    }
};

struct MouseDownEvent : public Event<MouseDownEvent> {
    // static constexpr const char* Name = "MouseDownEvent";
    int x;
    int y;
    MouseDownEvent(int x, int y) : x(x), y(y),  {};
};
struct CanvasUpdateEvent : public Event<CanvasUpdateEvent> {
    static constexpr const char* Name = "CanvasUpdateEvent";
    int pixelX, pixelY;
    uint32_t color;
    CanvasUpdateEvent(int x, int y, uint32_t color)
        : pixelX(x), pixelY(y), color(color) {}
};
struct ServerStateUpdateEvent : public Event<ServerStateUpdateEvent> {
    static constexpr const char* Name = "ServerStateUpdateEvent";
    int pixelX, pixelY;
    uint32_t color;

    ServerStateUpdateEvent(int x, int y, uint32_t color)
        : pixelX(x), pixelY(y), color(color) {}
};



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

};