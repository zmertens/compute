#ifndef GLUTILS_HPP
#define GLUTILS_HPP

#include <string>
#include <glad/glad.h>

#include "SDLHelper.hpp"

/**
 * This class is not intended to be initialized.
 * When using `checkForOpenGLError` wrap it around the function call to test.
 * @brief The GLUtils class
 */
class GLUtils
{
public:
    static bool CheckForOpenGLError(const std::string& file, int line);
    static void GlDebugCallback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* msg, const void* param);
};

#endif // GLUTILS_HPP
