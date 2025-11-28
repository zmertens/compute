#include "Compute.hpp"

#include <SDL3/SDL.h>

#include <glad/glad.h>

#define TOTAL_SPHERES 20
#define TOTAL_LIGHTS 5

const glm::vec3 Compute::CLEAR_COLOR = glm::vec3(0.f);
std::unordered_map<std::uint8_t, bool> Compute::mKepMap;


Compute::Compute()
    : mCamera(glm::vec3(0.0f, 50.0f, 200.0f), -90.0f, -10.0f, 65.0f, 0.1f, 500.0f)
      , mPlayer(mCamera)
{
    // Camera positioned above and in front of sphere circle
    // Looking towards center with slight downward pitch
    // Far plane set to 500 to see all spheres at radius ~125
}

void Compute::run()
{
    SDLHelper sdlHandler;
    bool success = sdlHandler.init();
    if (!success)
    {
        std::cout << "SDL Not initialized" << std::endl;
        return;
    }

    printOpenGlInfo();

    // Debug camera setup
    SDL_Log("Camera initialized at position: (%.2f, %.2f, %.2f)",
            mCamera.getPosition().x, mCamera.getPosition().y, mCamera.getPosition().z);
    SDL_Log("Camera target direction: (%.2f, %.2f, %.2f)",
            mCamera.getTarget().x, mCamera.getTarget().y, mCamera.getTarget().z);
    SDL_Log("Camera far plane: %.2f", mCamera.getFar());

    glEnable(GL_MULTISAMPLE);

    Shader tracerShader;
    tracerShader.compileAndAttachShader(ShaderTypes::VERTEX_SHADER, "./shaders/raytracer.vert.glsl");
    tracerShader.compileAndAttachShader(ShaderTypes::FRAGMENT_SHADER, "./shaders/raytracer.frag.glsl");
    tracerShader.linkProgram();
    tracerShader.bind();

    Shader computeShader;
    computeShader.compileAndAttachShader(ShaderTypes::COMPUTE_SHADER, "./shaders/raytracer.cs.glsl");
    computeShader.linkProgram();
    computeShader.bind();

    std::vector<Light> lights;
    std::vector<Sphere> spheres;
    Plane plane;

    GLuint vao;
    GLuint screenTex;
    GLuint shapeSSBO;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glGenTextures(1, &screenTex);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,
                   static_cast<GLsizei>(SDLHelper::GLFW_WINDOW_X),
                   static_cast<GLsizei>(SDLHelper::GLFW_WINDOW_Y));
    glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &shapeSSBO);

    initCompute(computeShader, shapeSSBO, spheres, plane, lights);

    constexpr float timePerFrame = 1.0f / 60.0f;
    float accumulator = 0.0f;
    unsigned int frameCounter = 0;
    float timeSinceLastUpdate = 0.0f;

    while (!sdlHandler.shouldClose())
    {
        sdlHandler.pollEvents();

        static double lastTime = SDLHelper::getTime();
        double currentTime = SDLHelper::getTime();
        auto deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        accumulator += deltaTime;

        while (accumulator >= timePerFrame)
        {
            accumulator -= timePerFrame;

            input(sdlHandler);

            update(deltaTime);
        }

        float ar = static_cast<float>(SDLHelper::GLFW_WINDOW_X) / static_cast<float>(SDLHelper::GLFW_WINDOW_Y);

        render(computeShader, tracerShader, spheres, ar, vao, screenTex);

        sdlHandler.swapBuffers();

        frameCounter++;
        timeSinceLastUpdate += deltaTime;

        // Update FPS display every ~1 second
        if (timeSinceLastUpdate >= 1.0f)
        {
            printFramesToConsole(sdlHandler, frameCounter, timeSinceLastUpdate);
            frameCounter = 0;
            timeSinceLastUpdate = 0.f;
        }
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &shapeSSBO);
    glDeleteTextures(1, &screenTex);

    sdlHandler.cleanUp();
}

void Compute::initCompute(Shader& compute, GLuint shapeSSBO,
                          std::vector<Sphere>& spheres, Plane& plane,
                          std::vector<Light>& lights)
{
    compute.bind();

    std::vector<glm::vec3> lightPositions = {
        glm::vec3(35.0f, 20.0f, -35.0f),
        glm::vec3(0.0f, 20.0f, 0.0f),
        glm::vec3(0.0f, 40.0f, 40.0f),
        glm::vec3(15.0f, 20.0f, -10.0f),
        glm::vec3(30.0f, 60.0f, 20.0f)
    };

    // lights
    for (unsigned int index = 0; index != TOTAL_LIGHTS; ++index)
    {
        glm::vec3 ambient(0.5f);
        glm::vec3 diffuse(Utils::getRandomFloat(0.09f, 1.0f),
                          Utils::getRandomFloat(0.09f, 1.0f),
                          Utils::getRandomFloat(0.09f, 1.0f));
        glm::vec3 specular(1.0f);

        glm::vec3 position = lightPositions.at(index);

        lights.emplace_back(ambient, diffuse, specular, glm::vec4(position, 0.0));

        compute.setUniform("uLights[" + Utils::toString(index) + "].position", glm::vec4(position, 0.0));
        compute.setUniform("uLights[" + Utils::toString(index) + "].ambient", ambient);
        compute.setUniform("uLights[" + Utils::toString(index) + "].diffuse", diffuse);
        compute.setUniform("uLights[" + Utils::toString(index) + "].specular", specular);
    }

    float imgCircleRadius = 125.0f;
    float offset = 15.25f;

    // Bind SSBO for sphere data - MUST be populated for shader to work
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shapeSSBO);
    // Allocate buffer space (will be filled after sphere creation)
    glBufferData(GL_SHADER_STORAGE_BUFFER, TOTAL_SPHERES * sizeof(Sphere), nullptr, GL_DYNAMIC_DRAW);

    // spheres
    for (unsigned int index = 0; index != TOTAL_SPHERES; ++index)
    {
        glm::vec3 ambient(Utils::getRandomFloat(0.09f, 1.0f),
                          Utils::getRandomFloat(0.09f, 1.0f), Utils::getRandomFloat(0.09f, 1.0f));
        glm::vec3 diffuse(Utils::getRandomFloat(0.1f, 0.90f),
                          Utils::getRandomFloat(0.09f, 0.9f), Utils::getRandomFloat(0.09f, 0.9f));
        glm::vec3 specular(Utils::getRandomFloat(0.5f, 1.0f),
                           Utils::getRandomFloat(0.5f, 1.0f), Utils::getRandomFloat(0.5f, 1.0f));

        float shiny = Utils::getRandomFloat(10.0f, 300.0f);
        float refl = Utils::getRandomFloat(0.05f, 1.0f);

        Material material(ambient, diffuse, specular, Utils::getRandomFloat(10.0f, 300.0f),
                          Utils::getRandomFloat(0.05f, 1.0f), Utils::getRandomFloat(0.05f, 1.0f));

        float angle = static_cast<float>(index) / static_cast<float>(TOTAL_SPHERES) * 360.0f;
        float angleRad = glm::radians(angle);
        float displacement = Utils::getRandomFloat(-offset, offset);

        float xpos = glm::sin(angleRad) * imgCircleRadius + displacement;
        displacement = Utils::getRandomFloat(-offset, offset);
        GLfloat y = std::abs(displacement) * 7.5f; // y value has smaller displacement
        displacement = Utils::getRandomFloat(-offset, offset);
        GLfloat z = glm::cos(angleRad) * imgCircleRadius + displacement;

        glm::vec3 center = glm::vec3(xpos, y, z);

        float radius = Utils::getRandomFloat(5.0f, 12.0f);
        spheres.emplace_back(center, radius, ambient, diffuse, specular, shiny, refl);

        compute.setUniform("uSpheres[" + Utils::toString(index) + "].material.ambient", material.getAmbient());
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].material.diffuse", material.getDiffuse());
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].material.specular", material.getSpecular());
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].material.shininess", material.getShininess());
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].material.reflective", material.getReflectivity());
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].center", center);
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].radius", radius);
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].radius2", radius * radius);
    }

    // NOW upload the complete spheres vector to SSBO - CRITICAL for rendering!
    glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_STATIC_DRAW);

#if defined(DEBUG_COMPUTE)
    // Verify SSBO data was uploaded correctly
    std::ofstream out;
    out.open("./sphere_data.txt");
    out << "Sphere SSBO Data Verification:\n";
    Sphere* ptr = (Sphere*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, spheres.size() * sizeof(Sphere),
                                            GL_MAP_READ_BIT);
    for (unsigned int i = 0; i != spheres.size(); ++i)
    {
        out << "Sphere[" << i << "], center(" << ptr[i].center.x << ", " << ptr[i].center.y << ", " << ptr[i].center.z
            << ")"
            << ", radius=" << ptr[i].radius << "\n";
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    out.close();
    std::cout << "Sphere data written to sphere_data.txt for debugging\n";
#endif // defined

    // plane
    glm::vec3 ambient(1.0f, 0.0f, 0.0f);
    glm::vec3 diffuse(1.0f, 0.0f, 0.0f);
    glm::vec3 specular(1.0f);
    Material planarMaterial(ambient, diffuse, specular, 250.0f, 0.0f, 0.0f);
    plane.material = planarMaterial;
    plane.normal = glm::vec3(0, 1, 0);
    plane.point = glm::vec3(0, -6, 0);

    //shader->setUniform("uPlane.material.ambient", planematerial.getAmbient());
    compute.setUniform("uPlane.material.diffuse", plane.material.getDiffuse());
    compute.setUniform("uPlane.material.specular", plane.material.getSpecular());
    compute.setUniform("uPlane.material.shininess", plane.material.getShininess());
    compute.setUniform("uPlane.material.reflective", plane.material.getReflectivity());
    compute.setUniform("uPlane.point", plane.point);
    compute.setUniform("uPlane.normal", plane.normal);
} // initCompute

void Compute::input(SDLHelper& sdlHandler)
{
    float mouseWheelDy = 0;

    double coordX = 0.0, coordY = 0.0;
    sdlHandler.getCursorPos(coordX, coordY);
    glm::vec2 coords = glm::vec2(coordX, coordY);

    // handle realtime input
    mPlayer.input(sdlHandler, mouseWheelDy, coords);
}

void Compute::update(const float dt)
{
    mPlayer.update(dt, 1.0);
}

/**
 * @type GL_TRIANGLE_STRIP
 */
void Compute::render(Shader& compute, Shader& raytracer, const std::vector<Sphere>& spheres, float ar,
                     GLuint vao, GLuint tex, GLenum type)
{
    glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    compute.bind();

    double time = SDLHelper::getTime();
    compute.setUniform("uTime", time);
    compute.setUniform("uCamera.eye", mCamera.getPosition());
    compute.setUniform("uCamera.far", mCamera.getFar());
    // aspect ratio is hardcoded which is not good
    compute.setUniform("uCamera.ray00", mCamera.getFrustumEyeRay(ar, -1, -1));
    compute.setUniform("uCamera.ray01", mCamera.getFrustumEyeRay(ar, -1, 1));
    compute.setUniform("uCamera.ray10", mCamera.getFrustumEyeRay(ar, 1, -1));
    compute.setUniform("uCamera.ray11", mCamera.getFrustumEyeRay(ar, 1, 1));

    // Get current time for sphere animation (removed 'static' to update every frame)
    auto elapsed = static_cast<float>(SDLHelper::getTime());
    for (unsigned int index = 0; index != TOTAL_SPHERES; ++index)
    {
        glm::mat4 transform;
        if (index % 2 == 0)
            transform = glm::translate(glm::vec3(glm::cos(elapsed) * 10.0f, glm::sin(elapsed) * 10.0f, 0.0f));
        else
            transform = glm::translate(glm::vec3(0.0f, glm::cos(elapsed) * 20.0f, glm::sin(elapsed) * 20.0f));

        // Get original center as vec3 and transform it
        glm::vec3 originalCenter = glm::vec3(spheres.at(index).center);
        glm::vec3 transformedCenter = glm::vec3(transform * glm::vec4(originalCenter, 1.0f));
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].center", transformedCenter);
    }

    glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(1080 / 20, 720 / 20, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    raytracer.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(vao);
    glDrawArrays(type, 0, 4);
} // render

void Compute::sdlEvents(SDLHelper& sdlHandler, float& mouseWheelDy, bool& running)
{
    // Event handling can be expanded here if needed
}

void Compute::printFramesToConsole(SDLHelper& sdlHandler, unsigned int frameCounter, float timeSinceLastUpdate) const noexcept
{
    if (timeSinceLastUpdate > 0.f && frameCounter > 0)
    {
        unsigned int fps = static_cast<unsigned int>(frameCounter / timeSinceLastUpdate);
        float msPerFrame = 1000.0f * timeSinceLastUpdate / static_cast<float>(frameCounter);

        // Update window title with FPS info
        char titleBuffer[256];
        snprintf(titleBuffer, sizeof(titleBuffer), "Compute Raytracer - FPS: %u | %.2f ms/frame", fps, msPerFrame);
        sdlHandler.setWindowTitle(titleBuffer);

        // Also log to console
        SDL_Log("FPS: %u | time (ms) / frame: %.2f\n", fps, msPerFrame);
    }
}

void Compute::printOpenGlInfo()
{
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    std::stringstream ss;
    ss << "\n-------------------------------------------------------------\n";
    ss << "GL Vendor    : " << vendor;
    ss << "\nGL GLRenderer : " << renderer;
    ss << "\nGL Version   : " << version;
    ss << "\nGL Version   : " << major << "." << minor;
    ss << "\nGLSL Version : " << glslVersion;
    ss << "\n-------------------------------------------------------------\n";
    printf("%s\n", ss.str().c_str());
}
