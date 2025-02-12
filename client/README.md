             (Event Flow)
                
                     [ Mouse Input ]
                            │
                            ▼
             ┌─────────────────────────────┐
             │           Canvas            │
             │    (logic + conflict res)   │
             └─────────────┬───────────────┘
                           │
            ┌──────────────┴───────────────┐
            │                              │
[CanvasLocalUpdateEvent]       [CanvasUiUpdateEvent]
            │                              │
            ▼                              ▼
   ┌─────────────────┐            ┌────────────────────┐
   │  NetTransport   │            │      Render        │
   │   (Network)     │            │  (WebGL/Scaling)   │
   └─────────┬───────┘            └────────────────────┘
             │
   [CanvasServerUpdateEvent]
             │
             ▼
   ┌─────────────────────────┐
   │   Authoritative Server  │
   │ (Sync & Conflict Res.)  │
   └─────────────────────────┘
