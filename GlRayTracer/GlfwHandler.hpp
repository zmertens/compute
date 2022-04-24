#ifndef GLFW_HANDLER
#define GLFW_HANDLER

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <array>

class GlfwHandler
{
public:
    GlfwHandler();
    GlfwHandler(const std::string& title, const std::string& iconFilepath);

    virtual bool init();
    virtual void cleanUp();

    void swapBuffers() const;

    std::string getGlString() const;
    GLFWwindow* getGlfwWindow() const;
    std::array<bool, 1024> getKeys() const;
    void resetKeys();

    static constexpr unsigned int GLFW_WINDOW_X = 640;
    static constexpr unsigned int GLFW_WINDOW_Y = 480;

private:
    static constexpr unsigned int GL_MAJOR_V = 4, GL_MINOR_V = 2;
    static constexpr unsigned int V_SYNC_FLAG = 1;
    static constexpr unsigned int GLFW_FULLSCREEN_FLAG = 0;

    GLFWwindow* mGlfwWindow;
    GLFWcursor* mGlfwCursor;
    std::string mGlslVersion;

    const std::string mTitle;
    const std::string mWindowIconPath;

    static std::array<bool, 1024> mInputs;

    static void setErrorCallback(int error, const char* description);
    static void setKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    unsigned char* loadImages(std::string filepath, int& x, int& y);
};

#endif // GLFW_HANDLER