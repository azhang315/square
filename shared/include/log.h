#pragma once
#include <spdlog/spdlog.h>
#include <string>

#ifndef SLOG
#define FILENAME (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define SLOG(msg, ...) spdlog::info("[{}:{}] " msg, FILENAME, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#endif