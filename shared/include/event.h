// event.h
#pragma once
#include <cstdint>

template <typename T>
struct Event
{
    T data;
    template <typename... Args>
    Event(Args &&...args) : data(std::forward<Args>(args)...) {}
    ~Event() = default;

    static const char *StaticName() { return T::Name; };
};
struct MouseDownEvent
{
    static constexpr const char *Name = "MouseDownEvent";
    int x;
    int y;
    uint32_t color;
    MouseDownEvent(int x, int y, uint32_t color) : x(x), y(y), color(color) {};
};
struct CanvasUiUpdateEvent
{
    static constexpr const char *Name = "CanvasUiUpdateEvent";
    int x, y;
    uint32_t color;
    CanvasUiUpdateEvent(int x, int y, uint32_t color) : x(x), y(y), color(color) {}
};
struct CanvasUiBatchUpdateEvent
{
    static constexpr const char *Name = "CanvasUIBatchUpdateEvent";
    int x, y;
    uint32_t color;
    CanvasUiBatchUpdateEvent(int x, int y, uint32_t color) : x(x), y(y), color(color) {}
};
struct CanvasLocalUpdateEvent
{
    static constexpr const char *Name = "CanvasLocalUpdateEvent";
    int x, y;
    uint32_t color;
    int seq;
    CanvasLocalUpdateEvent(int x, int y, uint32_t color, int seq) : x(x), y(y), color(color), seq(seq) {}
};

struct CanvasServerUpdateEvent
{
    static constexpr const char *Name = "CanvasServerUpdateEvent";
    int x, y;
    uint32_t color;
    int seq;
    CanvasServerUpdateEvent(int x, int y, uint32_t color, int seq) : x(x), y(y), color(color), seq(seq) {}
};
