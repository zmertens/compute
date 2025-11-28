#include "SDLHelper.hpp"
#include <glad/glad.h>
#include <iostream>

// Static member initialization
Uint64 SDLHelper::s_start_time = 0;

SDLHelper::SDLHelper() {
    m_key_state.fill(false);
}

SDLHelper::~SDLHelper() {
    if (m_window || m_context) {
        cleanUp();
    }
}

bool SDLHelper::init() {
    // SDL_Init returns true on SUCCESS (SDL3 behavior)
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    // Initialize start time
    s_start_time = SDL_GetPerformanceCounter();

    // Set app metadata (SDL3 feature)
    SDL_SetAppMetadata("Compute Raytracer", "1.0", nullptr);

    // Set OpenGL attributes
#if defined(DEBUG_COMPUTE)
    SDL_Log("Setting SDL_GL_CONTEXT_DEBUG_FLAG\n");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create window
    const Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
    m_window = SDL_CreateWindow("Compute Raytracer", GLFW_WINDOW_X, GLFW_WINDOW_Y, window_flags);

    if (m_window == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Create OpenGL context
    m_context = SDL_GL_CreateContext(m_window);
    if (m_context == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        SDL_Quit();
        return false;
    }

    // Make context current
    SDL_GL_MakeCurrent(m_window, m_context);

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize GLAD\n");
        cleanUp();
        return false;
    }

    // Set vsync
    SDL_GL_SetSwapInterval(1);

    // Show window
    SDL_ShowWindow(m_window);

    // Center window
    SDL_SetWindowPosition(m_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    SDL_Log("SDL initialized successfully: Compute Raytracer (%dx%d)\n", GLFW_WINDOW_X, GLFW_WINDOW_Y);

    return true;
}

void SDLHelper::pollEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
                m_should_close = true;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (e.key.scancode < 512) {
                    m_key_state[e.key.scancode] = true;
                }
                break;
            case SDL_EVENT_KEY_UP:
                if (e.key.scancode < 512) {
                    m_key_state[e.key.scancode] = false;
                }
                break;
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                m_should_close = true;
                break;
        }
    }
}

void SDLHelper::swapBuffers() {
    if (m_window) {
        SDL_GL_SwapWindow(m_window);
    }
}

void SDLHelper::cleanUp() {
    if (m_context) {
        SDL_GL_DestroyContext(m_context);
        m_context = nullptr;
    }

    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    if (SDL_WasInit(0) != 0) {
        SDL_Quit();
    }
}

void SDLHelper::getCursorPos(double& xpos, double& ypos) const {
    float x, y;
    SDL_GetMouseState(&x, &y);
    xpos = static_cast<double>(x);
    ypos = static_cast<double>(y);
}

void SDLHelper::setCursorPos(double xpos, double ypos) {
    if (m_window) {
        SDL_WarpMouseInWindow(m_window, static_cast<float>(xpos), static_cast<float>(ypos));
    }
}

double SDLHelper::getTime() {
    Uint64 current = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    return static_cast<double>(current - s_start_time) / static_cast<double>(frequency);
}

void SDLHelper::setWindowTitle(const char* title) {
    if (m_window) {
        SDL_SetWindowTitle(m_window, title);
    }
}

void SDLHelper::log_gl_info() const {
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    SDL_Log("-------------------------------------------------------------\n");
    SDL_Log("GL Vendor    : %s\n", vendor);
    SDL_Log("GL Renderer  : %s\n", renderer);
    SDL_Log("GL Version   : %s\n", version);
    SDL_Log("GL Version   : %d.%d\n", major, minor);
    SDL_Log("GLSL Version : %s\n", glslVersion);
    SDL_Log("-------------------------------------------------------------\n");
}

