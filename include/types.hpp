#pragma once

enum struct InputCode {
    MouseDown,
    MouseUp,
};
struct InputEvent {
    InputCode code;
    int x, y;
};

enum struct EventType {
    // Network Events
    // NetworkPacketRecv,
    NetworkUpdateErr, // Rollback / sync error
    ServerStateUpdate, // Authoritative Update


    // Input Events
    MouseDown,
    MouseUp,

    // Rendering
    CanvasUpdated, // Pixel update applied
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