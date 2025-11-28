#ifndef SDL_HELPER_H
#define SDL_HELPER_H

#include <SDL3/SDL.h>

#include <functional>
#include <mutex>

/// @brief SDL initialization and event handling helper
/// @details Encapsulates SDL subsystem initialization, window/context creation,
///          and input event processing. Separates SDL concerns from game logic.
///          Uses std::call_once for thread-safe one-time initialization.
class SDLHelper {
public:
    SDLHelper() = default;
    ~SDLHelper() = default;

    // Non-copyable
    SDLHelper(const SDLHelper&) = delete;
    SDLHelper& operator=(const SDLHelper&) = delete;

    /// @brief Initialize SDL with app metadata and create window/context (thread-safe, called once)
    /// @param title Window title and app name
    /// @param width Initial window width
    /// @param height Initial window height
    /// @param window Output parameter for created window
    /// @param context Output parameter for created GL context
    /// @return true if initialization succeeded, false otherwise
    bool init_once(std::string_view title, int width, int height,
                   SDL_Window*& window, SDL_GLContext& context) noexcept;

    /// @brief Set up window properties (position, icon, etc.)
    /// @param window SDL window to configure
    /// @param vsync Enable/disable vsync
    void configure_window(SDL_Window* window, bool vsync);

    /// @brief Handle SDL events and dispatch to callbacks
    /// @param imgui_process_event Function to process ImGui events (called first)
    /// @param event_callback Function to handle SDL events (called after ImGui)
    /// @return true to continue running, false to quit
    bool handle_events(const std::function<void(const SDL_Event&)>& imgui_process_event,
                      const std::function<bool(const SDL_Event&)>& event_callback);

    /// @brief Destroy window/context and shutdown SDL subsystems
    /// @param window Window to destroy (set to nullptr after destruction)
    /// @param context Context to destroy (set to nullptr after destruction)
    void destroy_and_quit(SDL_Window*& window, SDL_GLContext& context) noexcept;

    /// @brief Log OpenGL information
    void log_gl_info() const;

    /// @brief Log available display modes
    void log_display_modes() const;

private:
    std::once_flag m_sdl_init_flag;
    bool m_init_success{false};
};

} // namespace craft_input

#endif // SDL_HELPER_H

