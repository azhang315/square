#include <render.h>
#include <vector>
#include <emscripten/html5.h>
#include <cstdlib>
#include <ctime>
#include <emscripten/threading.h>

// Shader Compilation Helper
GLuint compile_shader(GLenum type, const char *source)
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    assert(!is_worker);

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        spdlog::error("Shader compilation error ({}): {}", (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment", log);
    }
    return shader;
}

// Program Linking Helper
GLuint create_program(GLuint vertexShader, GLuint fragmentShader)
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    assert(!is_worker);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        spdlog::error("Program linking error: {}", log);
    }
    return program;
}

// Vertex and Fragment Shaders
const char *vertexShaderSource = R"(#version 300 es
layout(location = 0) in vec2 aPos;
out vec2 TexCoord;
void main() {
    TexCoord = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char *fragmentShaderSource = R"(#version 300 es
precision mediump float;
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D uTexture;
void main() {
    FragColor = texture(uTexture, TexCoord);
}
)";

void adjust_canvas_resolution()
{
    double dpr = emscripten_get_device_pixel_ratio(); // Get the device pixel ratio

    int cssWidth, cssHeight;
    emscripten_get_canvas_element_size("#canvas", &cssWidth, &cssHeight);

    // Adjust canvas pixel buffer size (high DPI scaling)
    emscripten_set_canvas_element_size("#canvas", cssWidth * dpr, cssHeight * dpr);

    // Adjust the CSS size back to the original to maintain layout
    emscripten_set_element_css_size("#canvas", cssWidth, cssHeight);
}
void apply_webgl_scaling()
{
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_get_current_context();
    int drawingBufferWidth, drawingBufferHeight;
    emscripten_webgl_get_drawing_buffer_size(context, &drawingBufferWidth, &drawingBufferHeight);

    glViewport(0, 0, drawingBufferWidth, drawingBufferHeight); // Scale the viewport
}
EM_BOOL on_resize(int eventType, const EmscriptenUiEvent *e, void *userData)
{
    adjust_canvas_resolution();
    apply_webgl_scaling();
    return EM_TRUE;
}

// Render::Render(Canvas* canvas) : canvas(canvas)
Render::Render(int w, int h) : width(w), height(h), textureID(0), VAO(0), VBO(0), shaderProgram(0)
{
    init_gl();
    // adjust_canvas_resolution();
    // apply_webgl_scaling();
    // emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_FALSE, on_resize);
}

Render::~Render()
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    assert(!is_worker);

    if (textureID)
        glDeleteTextures(1, &textureID);
    if (VBO)
        glDeleteBuffers(1, &VBO);
    if (VAO)
        glDeleteVertexArrays(1, &VAO);
    if (shaderProgram)
        glDeleteProgram(shaderProgram);
}
void Render::init_gl()
{
    SLOG("init_gl()");
    //   std::shared_ptr<Render> *m_render_ptr = static_cast<std::shared_ptr<Render> *>(arg);
    //   (*m_render_ptr)->init_gl();
    emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VI, init_gl_impl, this);

    // auto _this = static_cast<Render*>(v_this);
    // _this->init_gl_impl();
}

void Render::init_gl_impl(void *v_this)
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    assert(!is_worker);

    auto _this = static_cast<Render *>(v_this);
    int width = _this->width, height = _this->height;
    // GLuint shaderProgram = _this->shaderProgram, textureID = _this->textureID, VBO = _this->VBO, VAO = _this->VAO;

    SLOG("init_gl_impl() - Setting up WebGL2");

    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = emscripten_webgl_create_context("#canvas", &attrs);
    if (context <= 0)
    {
        spdlog::error("Failed to create WebGL2 context");
        return;
    }
    emscripten_webgl_make_context_current(context);

    glViewport(0, 0, width, height); // TODO: uncomment if no scale

    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    _this->shaderProgram = create_program(vertexShader, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenTextures(1, &_this->textureID);
    glBindTexture(GL_TEXTURE_2D, _this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    std::vector<uint8_t> blankTexture(width * height * 4, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, blankTexture.data());
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    float vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f};
    glGenVertexArrays(1, &_this->VAO);
    glGenBuffers(1, &_this->VBO);
    glBindVertexArray(_this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, _this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Draw some dummy pixels for testing
    for (int i = 0; i < 100; ++i)
    {
        uint8_t pixel[4] = {static_cast<uint8_t>(std::rand() % 256), static_cast<uint8_t>(std::rand() % 256), static_cast<uint8_t>(std::rand() % 256), 255};
        _this->commit_to_gpu(pixel, i, i);
    }

    _this->draw(nullptr, false);

    SLOG("WebGL2 initialized successfully!");
}

void Render::commit_to_gpu(const void *pixelBuffer, int x, int y)
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    SLOG("ENIRONMENT_IS_WORKER: {}", is_worker);
    assert(!is_worker);

    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            spdlog::error("OpenGL Error after glTexSubImage2D: {}", error);
        }
    }
    else
    {
        spdlog::warn("Pixel coordinates ({}, {}) out of bounds!", x, y);
    }
}

void Render::draw(const void *pixelData, bool isDirty)
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    assert(!is_worker);

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLint textureLoc = glGetUniformLocation(shaderProgram, "uTexture");
    if (textureLoc != -1)
    {
        glUniform1i(textureLoc, 0);
    }
    else
    {
        spdlog::error("Uniform 'uTexture' not found.");
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        spdlog::error("OpenGL Error after glDrawArrays: {}", error);
    }
}

template void Render::handle_event<CanvasUiUpdateEvent>(EventPtr<CanvasUiUpdateEvent>);
template void Render::handle_event<CanvasUiBatchUpdateEvent>(EventPtr<CanvasUiBatchUpdateEvent>);

template <typename T>
void Render::handle_event(EventPtr<T> e)
{
    auto func_ptr = reinterpret_cast<void *>(Render::handle_event_impl<T>);
    emscripten_sync_run_in_main_runtime_thread(EM_FUNC_SIG_VII, func_ptr, this, &e);
}

template <>
inline void Render::handle_event_impl<CanvasUiUpdateEvent>(void *v_this, void *v_e)
{
    int is_worker = EM_ASM_INT(return ENVIRONMENT_IS_WORKER);
    assert(!is_worker);

    auto e = *static_cast<EventPtr<CanvasUiUpdateEvent> *>(v_e);
    auto *_this = static_cast<Render *>(v_this);

    if (!e || !(e.get()))
    {
        spdlog::error("Invalid event data!");
        return;
    }
    if (!_this)
    {
        spdlog::error("Invalid Render object pointer!");
        return;
    }

    SLOG("Render <- CANVAS UI");

    const auto &cue = e->data;
    SLOG("Drawing random pixel at ({}, {})", cue.x, cue.y);

    static bool seeded = false;
    if (!seeded)
    {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }

    uint8_t pixel[4] = {
        static_cast<uint8_t>(std::rand() % 256),
        static_cast<uint8_t>(std::rand() % 256),
        static_cast<uint8_t>(std::rand() % 256),
        255};

    if (cue.x >= 0 && cue.x < _this->width && cue.y >= 0 && cue.y < _this->height)
    {
        SLOG("cue.x = {}, cue.y = {}", cue.x, cue.y);

        _this->commit_to_gpu(pixel, cue.x, cue.y);
    }
    else
    {
        spdlog::warn("Pixel coordinates ({}, {}) out of bounds!", cue.x, cue.y);
    }

    _this->draw(nullptr, false);
}

template <>
inline void Render::handle_event_impl<CanvasUiBatchUpdateEvent>(void *v_this, void *v_e)
{
    SLOG("Render <- UI BATCH");
}
