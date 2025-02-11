
                            (Clients chosen via session management)
                   [ Client A ]         [ Client B ]         [ Client C ]
                        ▲                   X                   ▲
                        │                   │                   │
                ┌────────────────────────────────────────────────────┐
                │                    Network Layer                   │
                │               (net_transport.cpp)                  │
                └──────────────────────▲─────────────────────────────┘
                                       │
                      [ Prepare Unreliable Broadcast Packet ]
                                       │
                ┌────────────────────────────────────────────────────┐
                │              Event Dispatcher (Glue)              │
                │         (event_dispatcher.cpp - Fan-Out)          │
                └──────────────────────▲─────────────────────────────┘
                                       │
                        [ Trigger: Successful write-through ]
