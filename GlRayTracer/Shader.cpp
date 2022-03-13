#include "Shader.hpp"

#include <cstring>

#include "./extlibs/glm/gtc/type_ptr.hpp"

#include "Utils.hpp"

/**
 * @brief Shader::Shader
 */
Shader::Shader()
{
    createProgram();
}

/**
 * @brief Shader::~Shader
 */
Shader::~Shader()
{
    cleanUp();
}

/**
 * @brief Shader::compileAndAttachShader
 * @param shaderType
 * @param filename
 */
void Shader::compileAndAttachShader(const int shaderType, const std::string& filename)
{
    // Build shader string from a file
    std::string shaderCode = ""; 

    mFileNames.emplace(shaderType, filename);
    GLuint shaderId = compile(shaderType, shaderCode);

    attach(shaderId);

    deleteShader(shaderId);
}

/**
 * @brief Shader::compileAndAttachShader
 * @param shaderType
 * @param codeId
 * @param code
 */
void Shader::compileAndAttachShader(const int shaderType, const std::string& codeId, const GLchar* code)
{
    mFileNames.emplace(shaderType, codeId);
    GLuint shaderId = compile(shaderType, code);

    attach(shaderId);

    deleteShader(shaderId);
}

/**
 * @brief Shader::linkProgram
 */
void Shader::linkProgram()
{
    glLinkProgram(mProgram);

    GLint success;
    GLchar infoLog[512];

    glGetProgramiv(mProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(mProgram, 512, nullptr, infoLog);
        printf("Program link failed: %s\n", infoLog);
    }
}

/**
 * @brief Shader::bind
 */
void Shader::bind() const
{
    glUseProgram(mProgram);
}

/**
 * @brief Shader::release
 */
void Shader::release() const
{
    glUseProgram(0);
}

/**
 * @brief Shader::cleanUp
 */
void Shader::cleanUp()
{
    if (mProgram)
        deleteProgram(mProgram);
    mGlslLocations.clear();
    mFileNames.clear();
}

/**
 * @brief Shader::getGlslUniforms
 * @return
 */
std::string Shader::getGlslUniforms() const
{
#if APP_OPENGL_MAJOR >= 4 && APP_OPENGL_MINOR >= 3
    GLint numUniforms = 0;
    glGetProgramInterfaceiv(mProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
    GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};

    std::string retString = "\t(Active) GLSL Uniforms:\n";

    for (int i = 0; i != numUniforms; ++i)
    {
        GLint results[4];
        glGetProgramResourceiv(mProgram, GL_UNIFORM, i, 4, properties, 4, nullptr, results);

        if (results[3] != -1)
            continue; // skip block uniforms here

        GLint nameBufSize = results[0] + 1;
        char* name = new char[nameBufSize];

        glGetProgramResourceName(mProgram, GL_UNIFORM, i, nameBufSize, nullptr, name);
       SDL_Log("location = %d, name = %s, type = %s\n",
//            results[2], name, getStringFromType(results[1]).c_str());

        retString += "\tlocation = " + Utils::toString(results[2]) + ", name = "
            + name + ", type = " + getStringFromType(results[1]) + "\n";

        delete [] name;
    }

    return retString;
#endif
}

/**
 * @brief Shader::getGlslAttribs
 * @return
 */
std::string Shader::getGlslAttribs() const
{
#if APP_OPENGL_MAJOR >= 4 && APP_OPENGL_MINOR >= 3
    GLint numAttribs;
    glGetProgramInterfaceiv(mProgram, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);
    GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};

    std::string retString = "\t(Active) GLSL Attributes:\n";

    for (int i = 0; i != numAttribs; ++i)
    {
        GLint results[3];
        glGetProgramResourceiv(mProgram, GL_PROGRAM_INPUT, i, 3, properties, 3, nullptr, results);

        GLint nameBufSize = results[0] + 1;
        char* name = new char[nameBufSize];

        glGetProgramResourceName(mProgram, GL_PROGRAM_INPUT, i, nameBufSize, nullptr, name);
//        SDL_Log("location = %d, name = %s, type = %s\n",
//            results[2], name, getStringFromType(results[1]).c_str());

        retString += "\tlocation = " + Utils::toString(results[2]) + ", name = "
            + name + ", type = " + getStringFromType(results[1]) + "\n";

        delete [] name;
    }
    return retString;
#endif
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param matrix
 */
void Shader::setUniform(const std::string& str, const glm::mat3& matrix)
{
    glUniformMatrix3fv(getUniformLocation(str), 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param matrix
 */
void Shader::setUniform(const std::string& str, const glm::mat4& matrix)
{
    glUniformMatrix4fv(getUniformLocation(str), 1, GL_FALSE, glm::value_ptr(matrix));
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param vec
 */
void Shader::setUniform(const std::string& str, const glm::vec2& vec)
{
    glUniform2f(getUniformLocation(str), vec.x, vec.y);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param vec
 */
void Shader::setUniform(const std::string& str, const glm::vec3& vec)
{
    glUniform3f(getUniformLocation(str), vec.x, vec.y, vec.z);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param vec
 */
void Shader::setUniform(const std::string& str, const glm::vec4& vec)
{
    glUniform4f(getUniformLocation(str), vec.x, vec.y, vec.z, vec.w);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param arr
 * @param count
 */
void Shader::setUniform(const std::string& str, GLfloat arr[][2], unsigned int count)
{
    glUniform2fv(getUniformLocation(str), count, arr[0]);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param arr
 * @param count
 */
void Shader::setUniform(const std::string& str, GLint arr[], const unsigned int count)
{
    glUniform1iv(getUniformLocation(str), count, arr);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param arr
 * @param count
 */
void Shader::setUniform(const std::string& str, GLfloat arr[], unsigned int count)
{
    glUniform1fv(getUniformLocation(str), count, arr);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param value
 */
void Shader::setUniform(const std::string& str, GLfloat value)
{
    glUniform1f(getUniformLocation(str), value);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param value
 */
void Shader::setUniform(const std::string& str, GLdouble value)
{
    glUniform1d(getUniformLocation(str), value);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param value
 */
void Shader::setUniform(const std::string& str, GLint value)
{
    glUniform1i(getUniformLocation(str), value);
}

/**
 * @brief Shader::setUniform
 * @param str
 * @param value
 */
void Shader::setUniform(const std::string& str, GLuint value)
{
    glUniform1ui(getUniformLocation(str), value);
}

/**
 * @brief Shader::setSubroutine
 * @param shaderType
 * @param count
 * @param name
 */
void Shader::setSubroutine(GLenum shaderType, GLuint count, const std::string& name)
{
#if APP_OPENGL_MAJOR >= 4 && APP_OPENGL_MINOR >= 3
    GLuint loc = getSubroutineLocation(shaderType, name);
    glUniformSubroutinesuiv(shaderType, count, &loc);
#endif
}

/**
 * @brief Shader::setSubroutine
 * @param shaderType
 * @param count
 * @param index
 */
void Shader::setSubroutine(GLenum shaderType, GLuint count, GLuint index)
{
#if APP_OPENGL_MAJOR >= 4 && APP_OPENGL_MINOR >= 3
    glUniformSubroutinesuiv(shaderType, count, &index);
#endif
}

/**
 * @brief Shader::bindFragDataLocation
 * @param str
 * @param loc
 */
void Shader::bindFragDataLocation(const std::string& str, GLuint loc)
{
#if APP_OPENGL_MAJOR >= 4 && APP_OPENGL_MINOR >= 3
    glBindFragDataLocation(mProgram, loc, str.c_str());
#endif
}

/**
 * @brief Shader::bindAttribLocation
 * @param str
 * @param loc
 */
void Shader::bindAttribLocation(const std::string& str, GLuint loc)
{
    glBindAttribLocation(mProgram, loc, str.c_str());
}

/**
 * @brief Shader::getProgramHandle
 * @return
 */
unsigned int Shader::getProgramHandle() const
{
    return static_cast<unsigned int>(mProgram);
}

/**
 * @brief Shader::getShaderType
 * @param shaderType
 * @return
 */
GLenum Shader::getShaderType(const int shaderType) const
{
    switch (shaderType)
    {
        case ShaderTypes::VERTEX_SHADER:
            return GL_VERTEX_SHADER;
        case ShaderTypes::FRAGMENT_SHADER:
            return GL_FRAGMENT_SHADER;
        case ShaderTypes::GEOMETRY_SHADER:
          return GL_GEOMETRY_SHADER;
        case ShaderTypes::TESSELATION_CONTROL_SHADER:
            return GL_TESS_CONTROL_SHADER;
        case ShaderTypes::TESSELATION_EVAL_SHADER:
            return GL_TESS_EVALUATION_SHADER;
        case ShaderTypes::COMPUTE_SHADER:
          return GL_COMPUTE_SHADER;
    } // switch
}

/**
 * @brief Shader::getGlslLocations
 * @return
 */
std::unordered_map<std::string, GLint> Shader::getGlslLocations() const
{
    return mGlslLocations;
}

/**
 * @brief Shader::getFileLocations
 * @return
 */
std::unordered_map<int, std::string> Shader::getFileNames() const
{
    return mFileNames;
}

/**
 * @brief Shader::compile
 * @param shaderType
 * @param shaderCode
 * @return
 */
GLuint Shader::compile(const int shaderType, const std::string& shaderCode)
{
    GLint length = shaderCode.length();
    const GLchar* glShaderString = shaderCode.c_str();

    GLenum glShaderType = getShaderType(shaderType);

    GLint success;
    GLchar infoLog[512];

    GLuint shaderId = glCreateShader(glShaderType);

    glShaderSource(shaderId, 1, &glShaderString, &length);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        printf("%s -- Shader Compilation Failed: %s\n", mFileNames.at(shaderType).c_str(), infoLog);
    }
    else if (success)
    {
        printf("%s compiled successfully\n", mFileNames.at(shaderType).c_str(), infoLog);
    }

    return shaderId;
}

/**
 * @brief compile
 * @param shaderType
 * @param shaderCode
 * @return
 */
GLuint Shader::compile(const int shaderType, const GLchar* shaderCode)
{
    GLint length = sizeof(shaderCode) / sizeof(GLchar);

    GLenum glShaderType = getShaderType(shaderType);

    GLint success;
    GLchar infoLog[512];

    GLuint shaderId = glCreateShader(glShaderType);

    glShaderSource(shaderId, 1, &shaderCode, &length);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
        printf("%s -- Shader Compilation Failed: %s\n", mFileNames.at(shaderType).c_str(), infoLog);
    }
    else if (success)
    {
        printf("%s compiled successfully\n", mFileNames.at(shaderType).c_str(), infoLog);
    }

    return shaderId;

}

/**
 * @brief Shader::attach
 * @param shaderId
 */
void Shader::attach(GLuint shaderId)
{
    glAttachShader(mProgram, shaderId);
}

/**
 * @brief Shader::createProgram
 */
void Shader::createProgram()
{
    mProgram = glCreateProgram();
}

/**
 * @brief Shader::deleteShader
 * @param shaderId
 */
void Shader::deleteShader(GLuint shaderId)
{
    glDeleteShader(shaderId);
}

/**
 * @brief Shader::deleteProgram
 * @param shaderId
 */
void Shader::deleteProgram(GLint shaderId)
{
    glDeleteProgram(shaderId);
}

/**
 * @brief Shader::getUniformLocation
 * @param str
 * @return
 */
GLint Shader::getUniformLocation(const std::string& str)
{
    auto iter = mGlslLocations.find(str);
    if (iter == mGlslLocations.end())
    {
        GLint loc = glGetUniformLocation(mProgram, str.c_str());
        if (loc == -1)
        {
            printf("%s does not exist in the shader\n", str.c_str());
        }
        else
        {
            mGlslLocations.emplace(str, loc);
        }

        return loc;
    }
    else
    {
        return mGlslLocations.at(str);
    }
}

/**
 * @brief Shader::getAttribLocation
 * @param str
 * @return
 */
GLint Shader::getAttribLocation(const std::string& str)
{
    return glGetAttribLocation(mProgram, str.c_str());
}

/**
 * @brief Shader::getSubroutineLocation
 * @param shaderType
 * @param name
 * @return
 */
GLuint Shader::getSubroutineLocation(GLenum shaderType, const std::string& name)
{
#if APP_OPENGL_MAJOR >= 4 && APP_OPENGL_MINOR >= 3
    return glGetSubroutineIndex(mProgram, shaderType, name.c_str());
#else
    return -1;
#endif
}

/**
 * @brief Shader::getStringFromType
 * @param type
 * @return
 */
std::string Shader::getStringFromType(GLenum type) const
{
    switch (type)
    {
        case GL_FLOAT: return std::string("float");
        case GL_FLOAT_VEC2: return std::string("vec2");
        case GL_FLOAT_VEC3: return std::string("vec3");
        case GL_FLOAT_VEC4: return std::string("vec4");
#if APP_OPENGL_MAJOR >= 4 && APP_OPENGL_MINOR >= 3
        case GL_DOUBLE: return std::string("double");
#endif
        case GL_INT: return std::string("int");
        case GL_UNSIGNED_INT: return std::string("unsigned int");
        case GL_BOOL: return std::string("bool");
        case GL_FLOAT_MAT2: return std::string("mat2");
        case GL_FLOAT_MAT3: return std::string("mat3");
        case GL_FLOAT_MAT4: return std::string("mat4");
        default: return std::string("Unknown GLenum type.");
    }
}
