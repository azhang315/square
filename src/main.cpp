// main.cpp
#include <iostream>
#include <emscripten.h>
#include <application.h>
#include <memory>
#include <spdlog/spdlog.h>




int main() {
    spdlog::info("Entry point: main()");
    Application app;
    app.run();
    return 0;
}
