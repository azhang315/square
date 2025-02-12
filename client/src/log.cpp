// log.cpp
#include <log.h>
extern "C" {
    void test_server_log() {
        spdlog::info("Test log from WASM entry point");
    }
}