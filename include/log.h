#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>


#include <filesystem>
namespace fs = std::filesystem;

// void setup_logger() {
//     if (!fs::exists("logs")) {
//         fs::create_directory("logs");
//     }
//     auto logger = spdlog::basic_logger_mt("file_logger", "logs/app.log");
//     spdlog::set_default_logger(logger);
//     spdlog::set_level(spdlog::level::debug);
//     spdlog::flush_on(spdlog::level::info);  // Ensure logs flush immediately
//     spdlog::info("Logger Initialized (Directory check passed)");
// }

extern "C" {
    void test_server_log() {
        spdlog::info("Test log from WASM entry point");
    }
}