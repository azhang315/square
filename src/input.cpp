// input.cpp
#include <input.h>
#include <iostream>
#include <emscripten/html5.h>
#include <event.h>
#include <spdlog/spdlog.h>
#include <canvas.h>
#include <emscripten/val.h>



Input::Input() {
    spdlog::info("Input::init()");
    emscripten_set_mousedown_callback("#canvas", this, true, on_mouse_down);
}

// EM_BOOL Input::on_mouse_down(int eventType, const EmscriptenMouseEvent *e, void *userData)
// {
//     spdlog::info("Event: on_mouse_down");
    
//     Input *input = static_cast<Input *>(userData);
//     if (input)
//     {
//         spdlog::info("clientX: {}, clientY: {}", e->clientX, e->clientY);
//         spdlog::info("canvasX: {}, canvasY: {}", e->canvasX, e->canvasY);
//         spdlog::info("targetX: {}, targetY: {}", e->targetX, e->targetY);
//         spdlog::info("screenX: {}, screenY: {}", e->screenX, e->screenY);

//         double dpr = emscripten_get_device_pixel_ratio(); // good
//         double cssWidth, cssHeight;
//         emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);

//         int canvasWidth, canvasHeight;
//         emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
        

//         // Convert client coordinates to canvas coordinates
//         double canvasX = (e->clientX - rect.x) * (Canvas::WIDTH / rect.width);
//         double canvasY = (e->clientY - rect.y) * (Canvas::HEIGHT / rect.height);

//         // Flip Y-axis for WebGL
//         int flippedY = Canvas::HEIGHT - static_cast<int>(canvasY) - 1;

//         spdlog::info("Mapped Coordinates: canvasX = {}, canvasY (flipped) = {}", static_cast<int>(canvasX), flippedY);

//         MouseDownEvent event(static_cast<int>(canvasX), flippedY, 0);
//         input->notify_listeners(event);
//     }
//     return EM_TRUE;
// }


EM_BOOL Input::on_mouse_down(int eventType, const EmscriptenMouseEvent *e, void *userData)
{
    Input *input = static_cast<Input *>(userData);
    if (input)
    {
        // Step 1: Get the CSS size of the canvas
        double cssWidth, cssHeight;
        emscripten_get_element_css_size("#canvas", &cssWidth, &cssHeight);

        // Step 2: Get the WebGL drawing buffer size
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_get_current_context();
        int drawingBufferWidth, drawingBufferHeight;
        emscripten_webgl_get_drawing_buffer_size(context, &drawingBufferWidth, &drawingBufferHeight);

        // Step 3: Get the canvas position offset relative to the viewport
        // EmscriptenBoundingClientRect rect;
        // emscripten_get_element_bounding_client_rect("#canvas", &rect);

        // Step 4: Normalize mouse coordinates
        double normalizedX = ((e->targetX) / cssWidth) * drawingBufferWidth;
        double normalizedY = ((e->targetY) / cssHeight) * drawingBufferHeight;

        // Step 5: Flip Y-axis for WebGL
        int flippedY = drawingBufferHeight - static_cast<int>(normalizedY) - 1;

        // Dispatch the event
        MouseDownEvent event(static_cast<int>(normalizedX), flippedY, 0);
        input->notify_listeners(event);
    }
    return EM_TRUE;
}


// static EM_BOOL on_mouse_up(int eventType, const EmscriptenMouseEvent* e, void* userData) {
//     Input* input = static_cast<Input*>(userData);
//     if (input) {
//         Event event(EventType::MouseUp, EventMouseUp{e->clientX, e->clientY});
//         input->notify_listeners(event);
//     }
//     return EM_TRUE;
// }

// Process queued input events
// void Input::update() {
//     while (!m_event_queue.empty()) {
//         InputEvent event = m_event_queue.front();
//         m_event_queue.pop();

//         process_event(event); // Call internal handler
//     }
// }

// inline void Input::queue_one_event(InputEvent event) {
//     m_event_queue.push(event); // network
// }

// bool Input::is_mouse_down() const {
//     auto it = m_current_states.find(InputCode::MouseDown);
//     return it != m_current_states.end() && it->second;
// }

// bool Input::is_mouse_down_current_frame() const {
//     // Track state changes for single-frame detection
//     return false; // Implement based on your frame state tracking
// }

// bool Input::poll_one_event(InputEvent& out_event) {
//     if (!m_event_queue.empty()) {
//         out_event = m_event_queue.front();
//         m_event_queue.pop();
//         return true;
//     }
//     return false;
// }

// // Batched Events
// void Input::process_event(const InputEvent& e) {
//     switch (event.code) {
//         // MouseDown, MouseUp served immediately
//         // case InputCode::MouseDown:
//         // process_event_mouse_down(event);
//         // EventNotifierMixIn::notify_listeners(e_type, e_data); // e.g., call Listeners (canvas, net) functions
//         // break;
//         // case InputCode::MouseUp:
//         // process_event_mouse_up(event);
//         // EventNotifierMixIn::notify_listeners(e_type, e_data);
//         // break;

//         case EventType::NetworkUpdateErr:
//     }
// }

// static inline void process_event_mouse_down(InputEvent event) {
//     // m_current_states[InputCode::MouseDown] = true;

//     canvas_manager->draw_pixel(x, y, color);
//     net_transport->send_mouse_event(x, y, color);
// }
// static inline void process_event_mouse_up(InputEvent event) {
//     m_current_states[InputCode::MouseDown] = false;
// }
