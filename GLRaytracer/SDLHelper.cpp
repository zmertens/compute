#include "sdl_helper.h"
#include <glad/glad.h>

namespace craft_input {

bool sdl_helper::init_once(std::string_view title, int width, int height,
                            SDL_Window*& window, SDL_GLContext& context) noexcept {
    auto init_func = [this, title, width, height, &window, &context]() {
        // Set app metadata (SDL3 feature)
        if (!SDL_SetAppMetadata("MazeBuilder Voxels", title.data(), "voxel;maze;c++;sdl;opengl")) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_SetAppMetadata failed: %s\n", SDL_GetError());
            m_init_success = false;
            return;
        }

        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, "https://github.com/zmertens/MazeBuilder");
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "MazeBuilder");
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "MIT License");
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "simulation;game;voxel");

        // Set OpenGL attributes
#if defined(MAZE_DEBUG)
        SDL_Log("Setting SDL_GL_CONTEXT_DEBUG_FLAG\n");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
#endif

#if defined(__EMSCRIPTEN__)
        SDL_Log("Setting SDL_GL_CONTEXT_PROFILE_ES\n");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#else
        SDL_Log("Setting SDL_GL_CONTEXT_PROFILE_CORE\n");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

        // Create window
        const Uint32 window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE;
        window = SDL_CreateWindow(title.data(), width, height, window_flags);

        if (window == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow failed: %s\n", SDL_GetError());
            m_init_success = false;
            return;
        }

        // Create OpenGL context
        context = SDL_GL_CreateContext(window);
        if (context == nullptr) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
            SDL_DestroyWindow(window);
            window = nullptr;
            m_init_success = false;
            return;
        }

        // Make context current
        SDL_GL_MakeCurrent(window, context);

        SDL_Log("SDL initialized successfully: %s (%dx%d)\n", title.data(), width, height);
        m_init_success = true;
    };

    // SDL_Init returns true on SUCCESS (SDL3 behavior)
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::call_once(m_sdl_init_flag, init_func);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s\n", SDL_GetError());
        m_init_success = false;
    }

    return m_init_success;
}

void sdl_helper::configure_window(SDL_Window* window, bool vsync) {
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Cannot configure null window\n");
        return;
    }

    // Show window
    SDL_ShowWindow(window);

    // Disable relative mouse mode initially
    SDL_SetWindowRelativeMouseMode(window, false);

    // Center window
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    // Set vsync
    SDL_GL_SetSwapInterval(vsync);

    // Load and set icon
    const char* icon_path = "textures/maze_in_green_32x32.bmp";
    SDL_Surface* icon_surface = SDL_LoadBMP_IO(SDL_IOFromFile(icon_path, "rb"), true);
    if (icon_surface) {
        SDL_SetWindowIcon(window, icon_surface);
        SDL_DestroySurface(icon_surface);
    } else {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ERROR: Couldn't load icon at %s\n", icon_path);
    }
}

bool sdl_helper::handle_events(const std::function<void(const SDL_Event&)>& imgui_process_event,
                               const std::function<bool(const SDL_Event&)>& event_callback) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        // Process ImGui events first
        if (imgui_process_event) {
            imgui_process_event(e);
        }

        // Then process game events
        if (event_callback) {
            if (!event_callback(e)) {
                return false; // Quit requested
            }
        }
    }

    return true; // Continue running
}

void sdl_helper::destroy_and_quit(SDL_Window*& window, SDL_GLContext& context) noexcept {
    // Prevent double-destruction
    if (!window && !context) {
        SDL_Log("sdl_helper::destroy_and_quit() - Already destroyed, skipping\n");
        return;
    }

    if (context) {
#if defined(MAZE_DEBUG)
        SDL_Log("sdl_helper::destroy_and_quit() - Destroying GL context %p\n", static_cast<void*>(context));
#endif
        SDL_GL_DestroyContext(context);
        context = nullptr;
    }

    if (window) {
#if defined(MAZE_DEBUG)
        SDL_Log("sdl_helper::destroy_and_quit() - Destroying window %p\n", static_cast<void*>(window));
#endif
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    // Only call SDL_Quit() if SDL was actually initialized
    if (SDL_WasInit(0) != 0) {
        SDL_Log("sdl_helper::destroy_and_quit() - Calling SDL_Quit()\n");
        SDL_Quit();
    }
}

void sdl_helper::log_gl_info() const {
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

void sdl_helper::log_display_modes() const {
    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    int num_modes = 0;
    const SDL_DisplayMode* const* modes = SDL_GetFullscreenDisplayModes(display, &num_modes);
    if (modes) {
        for (int i = 0; i < num_modes; ++i) {
            const SDL_DisplayMode* mode = modes[i];
            SDL_Log("Display %" SDL_PRIu32 " mode %d: %dx%d@%gx %gHz\n",
                display, i, mode->w, mode->h, mode->pixel_density, mode->refresh_rate);
        }
    }
}

} // namespace craft_input

