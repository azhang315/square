#pragma once
#include <spdlog/spdlog.h>
#include <string>

#ifndef SLOG
// #define FILENAME (std::string(__FILE__).substr(std::string(__FILE__).find_last_of("/") + 1))
#define FILENAME (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

// #define SLOG(msg, ...) spdlog::info("[{}:{}] " msg, FILENAME, __LINE__, ##__VA_ARGS__)
#define SLOG(msg, ...) spdlog::info("[{}:{}] " msg, FILENAME, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#endif

// extern "C" {
//     void test_server_log();
// }

// #include <spdlog/sinks/basic_file_sink.h>

// #include <filesystem>
// namespace fs = std::filesystem;

// void setup_logger() {
//     if (!fs::exists("logs")) {
//         fs::create_directory("logs");
//     }
//     auto logger = spdlog::basic_logger_mt("file_logger", "logs/app.log");
//     spdlog::set_default_logger(logger);
//     spdlog::set_level(spdlog::level::debug);
//     spdlog::flush_on(spdlog::level::info);  // Ensure logs flush immediately
//     SLOG("Logger Initialized (Directory check passed)");
// }