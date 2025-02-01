// main.cpp
#include <iostream>
#include <emscripten.h>
#include "include/application.hpp"

#ifndef FPS
    #define FPS 60
#endif

int main() {
    Application app;
    emscripten_set_main_loop(app.one_iter, FPS, true);
    return 0;
}
