// main.cpp
#include <iostream>
#include <emscripten.h>
#include <application.h>
#include <memory>
#include <emscripten/threading.h>
#include <emscripten/proxying.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <emscripten/em_asm.h>
#include <emscripten/eventloop.h>
#include <emscripten/threading.h>
#include <log.h>

// TODO: add validation of required compile-time directives

int main()
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    assert(is_worker);

    if (!emscripten_has_threading_support())
    {
        SLOG("Error: Pthreads not supported in this build!");
        return 1;
    }

    spdlog::set_pattern("[%^%l%$] %v");

    Application app;
    app.run();
    assert(false);
    return 0;
}