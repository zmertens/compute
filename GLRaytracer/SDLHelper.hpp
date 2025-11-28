#ifndef SDL_HELPER_H
#define SDL_HELPER_H

#include <SDL3/SDL.h>
#include <array>
#include <functional>
#include <mutex>

/// @brief SDL initialization and event handling helper
/// @details Encapsulates SDL subsystem initialization, window/context creation,
///          and input event processing. Provides GLFW-compatible API for easier migration.
class SDLHelper {
public:
    // Window dimensions (legacy GLFW compatibility)
    static constexpr int GLFW_WINDOW_X = 1280;
    static constexpr int GLFW_WINDOW_Y = 720;

    SDLHelper();
    ~SDLHelper();

    // Non-copyable
    SDLHelper(const SDLHelper&) = delete;
    SDLHelper& operator=(const SDLHelper&) = delete;

    // GLFW-compatible API
    /// @brief Initialize SDL and create window/context (GLFW-compatible)
    /// @return true if initialization succeeded, false otherwise
    bool init();

    /// @brief Get the SDL window handle
    /// @return Pointer to SDL_Window
    SDL_Window* getWindow() const { return m_window; }

    /// @brief Check if window should close (GLFW-compatible)
    /// @return true if quit was requested, false otherwise
    bool shouldClose() const { return m_should_close; }

    /// @brief Poll SDL events (GLFW-compatible)
    void pollEvents();

    /// @brief Swap OpenGL buffers (GLFW-compatible)
    void swapBuffers();

    /// @brief Clean up and shutdown SDL (GLFW-compatible)
    void cleanUp();

    /// @brief Get keyboard state (GLFW-compatible)
    /// @return Array of key states indexed by SDL_Scancode
    const std::array<bool, 512>& getKeys() const { return m_key_state; }

    /// @brief Get cursor position (GLFW-compatible)
    /// @param xpos Output parameter for x coordinate
    /// @param ypos Output parameter for y coordinate
    void getCursorPos(double& xpos, double& ypos) const;

    /// @brief Set cursor position (GLFW-compatible)
    /// @param xpos X coordinate
    /// @param ypos Y coordinate
    void setCursorPos(double xpos, double ypos);

    /// @brief Get time in seconds since initialization (GLFW-compatible)
    /// @return Time in seconds
    static double getTime();

    /// @brief Set window title
    /// @param title New window title
    void setWindowTitle(const char* title);

    /// @brief Log OpenGL information
    void log_gl_info() const;

private:
    SDL_Window* m_window{nullptr};
    SDL_GLContext m_context{nullptr};
    std::array<bool, 512> m_key_state{};
    bool m_should_close{false};
    static Uint64 s_start_time;
};

#endif // SDL_HELPER_H

