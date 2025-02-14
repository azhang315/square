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

_Atomic bool done_callback = false;
// void main_thread_callback(void *user_data)
// {
//   printf("CALLBACK main_thread_callback called\n");

//   int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
//   printf("CALLBACK: ENVIRONMENT_IS_WORKER: %d\n", is_worker);

//   done_callback = true;
// }

// void set_timeout_on_main()
// {
//   int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
//   printf("main: ENVIRONMENT_IS_WORKER: %d\n", is_worker);
//   // Verify that we can do async work here on the main thread
//   // without causing the runtime to exit.
//   emscripten_set_immediate(main_thread_callback, NULL);
// }




int main()
{
  int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
  printf("ENVIRONMENT_IS_WORKER: %d\n", is_worker);
  assert(is_worker);

  if (!emscripten_has_threading_support())
  {
    SLOG("Error: Pthreads not supported in this build!");
    return 1;
  }
  // #define LOG(x) std::cout << __FILE__ << ":" << __LINE__ << " " << x << std::endl;
  // LOG("HELLO WORLD");
  spdlog::set_pattern("[%^%l%$] %v");
  SLOG("Hello, world!");


  Application app;
  app.run();
  assert(false);
  return 0;
}

/*
// main.cpp
#include <iostream>
#include <emscripten.h>
#include <application.h>
#include <memory>
#include <spdlog/spdlog.h>
#include <emscripten/threading.h>
#include <emscripten/proxying.h>

void run_on_main_thread() {
    std::cout << "This is running on the main browser thread." << std::endl;
    SLOG("yyy");

    // Perform operations that require the main thread here
}

int main() {
    std::cout << "Main function started on a pthread." << std::endl;
    SLOG("xxx");

    emscripten_main_thread_process_queued_calls();
    emscripten_proxy_execute_queue();
    auto id = emscripten_main_runtime_thread_id();

    auto q = em_proxying_queue_create();
    emscripten_proxy_sync(q, id);


    // Proxy the function to run on the main browser thread
    emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_V, run_on_main_thread);

    std::cout << "Main function completed on a pthread." << std::endl;
}

*/

/*
void* workerFunction(void* arg) {
    SLOG("Hello from Worker Thread!");
    return nullptr;
}

int main() {
    SLOG("Entry point: main()");

    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    SLOG("ENVIRONMENT_IS_WORKER: %d\n", is_worker);
    assert(is_worker);


    if (!emscripten_has_threading_support()) {
        SLOG("Error: Pthreads not supported in this build!");
        return 1;
    }
    pthread_t thread;
    if (pthread_create(&thread, nullptr, workerFunction, nullptr) != 0) {
        SLOG("Error: Failed to create thread");
        return 1;
    }
    pthread_join(thread, nullptr); // Wait for thread to finish
    return  0;



    Application app;
    app.run();
    return 0;
}
*/