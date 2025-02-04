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
    NetworkUpdateErr,
    NetworkUpdateSync,
    MouseDown,
};