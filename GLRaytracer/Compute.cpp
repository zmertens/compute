#include "Compute.hpp"

#include <glad/glad.h>

#define TOTAL_SPHERES 20
#define TOTAL_LIGHTS 5

const glm::vec3 Compute::CLEAR_COLOR = glm::vec3(0.f);
std::unordered_map<std::uint8_t, bool> Compute::mKepMap;


Compute::Compute()
: mCamera(glm::vec3(-100.f, 25.f, -100.f))
, mPlayer(mCamera)
{

}

void Compute::run()
{
    SDLHelper glfwHandler;
    bool success = glfwHandler.init();
    if (!success) {
        std::cout << "GLFW Not initialized" << std::endl;
    }

    printOpenGlInfo();

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

    const float timePerFrame = 1.0f / 60.0f;
    float accumulator = 0.0f;
    unsigned int frameCounter = 0;
    float timeSinceLastUpdate = 0.0f;
    float mouseWheelDelta = 0.0f;
    bool running = true;
    while (!glfwWindowShouldClose(glfwHandler.getGlfwWindow()))
    {
        glfwPollEvents();

        // Reset input keys to false ever frame
        auto&& inputs = std::move(glfwHandler.getKeys());
        for (auto input : inputs) {
            input = false;
        }

        static double lastTime = static_cast<double>(glfwGetTime()) / 1000.0;
        double currentTime = static_cast<double>(glfwGetTime()) / 1000.0;
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        accumulator += deltaTime;

        // while (accumulator > timePerFrame)
        // {
            accumulator -= timePerFrame;
            input(glfwHandler, mouseWheelDelta, running);
            update(deltaTime, timePerFrame);
        // }

        float ar = static_cast<float>(SDLHelper::GLFW_WINDOW_X) / static_cast<float>(SDLHelper::GLFW_WINDOW_Y);

        render(computeShader, tracerShader, spheres, ar, vao, screenTex);

        glfwHandler.swapBuffers();
        // glfwPollEvents();
        printFramesToConsole(frameCounter, timeSinceLastUpdate, deltaTime);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &shapeSSBO);
    glDeleteTextures(1, &screenTex);

    glfwHandler.cleanUp();


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
        glm::vec3 ambient (0.5f);
        glm::vec3 diffuse (Utils::getRandomFloat(0.09f, 1.0f),
            Utils::getRandomFloat(0.09f, 1.0f),
            Utils::getRandomFloat(0.09f, 1.0f));
        glm::vec3 specular (1.0f);

        glm::vec3 position = lightPositions.at(index);

        lights.emplace_back(ambient, diffuse, specular, glm::vec4(position, 0.0));

        compute.setUniform("uLights[" + Utils::toString(index) + "].position", glm::vec4(position, 0.0));
        compute.setUniform("uLights[" + Utils::toString(index) + "].ambient", ambient);
        compute.setUniform("uLights[" + Utils::toString(index) + "].diffuse", diffuse);
        compute.setUniform("uLights[" + Utils::toString(index) + "].specular", specular);
    }

    float imgCircleRadius = 125.0f;
    float offset = 15.25f;

#if defined(DEBUG_COMPUTE)
    GLUtils::CheckForOpenGLError(__FILE__, __LINE__);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shapeSSBO);
    GLUtils::CheckForOpenGLError(__FILE__, __LINE__);
    glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_STATIC_COPY);
    GLUtils::CheckForOpenGLError(__FILE__, __LINE__);
#endif // defined

    // spheres
    for (unsigned int index = 0; index != TOTAL_SPHERES; ++index)
    {
        glm::vec3 ambient (Utils::getRandomFloat(0.09f, 1.0f),
            Utils::getRandomFloat(0.09f, 1.0f), Utils::getRandomFloat(0.09f, 1.0f));
        glm::vec3 diffuse (Utils::getRandomFloat(0.1f, 0.90f),
            Utils::getRandomFloat(0.09f, 0.9f), Utils::getRandomFloat(0.09f, 0.9f));
        glm::vec3 specular (Utils::getRandomFloat(0.5f, 1.0f),
            Utils::getRandomFloat(0.5f, 1.0f), Utils::getRandomFloat(0.5f, 1.0f));

        float shiny = Utils::getRandomFloat(10.0f, 300.0f);
        float refl = Utils::getRandomFloat(0.05f, 1.0f);

        Material material (ambient, diffuse, specular, Utils::getRandomFloat(10.0f, 300.0f), Utils::getRandomFloat(0.05f, 1.0f), Utils::getRandomFloat(0.05f, 1.0f));

        float angle = static_cast<float>(index) / static_cast<float>(TOTAL_SPHERES) * 360.0f;
        float displacement = Utils::getRandomFloat(-offset, offset);

        float xpos = glm::sin(angle) * imgCircleRadius + displacement;
        displacement = Utils::getRandomFloat(-offset, offset);
        GLfloat y = std::abs(displacement) * 7.5f; // y value has smaller displacement
        displacement = Utils::getRandomFloat(-offset, offset);
        GLfloat z = glm::cos(angle) * imgCircleRadius + displacement;

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

#if defined(DEBUG_COMPUTE)
    GLUtils::CheckForOpenGLError(__FILE__, __LINE__);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shapeSSBO);
    GLUtils::CheckForOpenGLError(__FILE__, __LINE__);
    glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_STATIC_COPY);
    GLUtils::CheckForOpenGLError(__FILE__, __LINE__);
    std::ofstream out;
    out.open("./sphere_data.txt");
    out << "Origin sphere:\n";
    out << "\nMapped Sphere data:\n";
    Sphere* ptr = (Sphere*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, spheres.size() * sizeof(Sphere), GL_MAP_READ_BIT);
    for (unsigned int i = 0; i != spheres.size(); ++i)
    {
        out << "Sphere[" << i << "], center(" << ptr[i].center.x << ", " << ptr[i].center.y << ", " << ptr[i].center.z << ")\n";
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    out.close();
#endif // defined

    // plane
    glm::vec3 ambient (1.0f, 0.0f, 0.0f);
    glm::vec3 diffuse (1.0f, 0.0f, 0.0f);
    glm::vec3 specular (1.0f);
    Material planarMaterial (ambient, diffuse, specular, 250.0f, 0.0f, 0.0f);
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

void Compute::input(SDLHelper& glfwHandler, const float mouseWheelDelta, bool& running)
{
    float mouseWheelDy = 0;

    double coordX = 0.0, coordY = 0.0;
    glfwGetCursorPos(glfwHandler.getGlfwWindow(), &coordX, &coordY);
    glm::vec2 coords = glm::vec2(coordX, coordY);

    // handle realtime input
    mPlayer.input(glfwHandler, mouseWheelDy, coords);
}

void Compute::update(const float dt, const double timeSinceInit)
{
    mPlayer.update(dt, timeSinceInit);
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

    double time = static_cast<double>(glfwGetTime()) / 1000.0;
    compute.setUniform("uTime", time);
    compute.setUniform("uCamera.eye", mCamera.getPosition());
    compute.setUniform("uCamera.far", mCamera.getFar());
    // aspect ratio is hardcoded which is not good
    compute.setUniform("uCamera.ray00", mCamera.getFrustumEyeRay(ar, -1.0f, -1.0f));
    compute.setUniform("uCamera.ray01", mCamera.getFrustumEyeRay(ar, -1.0f, 1.0f));
    compute.setUniform("uCamera.ray10", mCamera.getFrustumEyeRay(ar, 1.0f, -1.0f));
    compute.setUniform("uCamera.ray11", mCamera.getFrustumEyeRay(ar, 1.0f, 1.0f));

   static double elapsed = static_cast<double>(glfwGetTime()) / 1000.0;
   for (unsigned int index = 0; index != TOTAL_SPHERES; ++index)
   {
        glm::mat4 transform;
        if (index % 2 == 0)
            transform = glm::translate(glm::vec3(glm::cos(elapsed) * 10.0f, glm::sin(elapsed) * 10.0f, 0));
        else
            transform = glm::translate(glm::vec3(0, glm::cos(elapsed) * 20.0f, glm::sin(elapsed) * 20.0f));
        compute.setUniform("uSpheres[" + Utils::toString(index) + "].center", 
            glm::vec3(transform * glm::vec4(spheres.at(index).center.x, spheres.at(index).center.y, spheres.at(index).center.z, 1.0)));
    }

    glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(1080 / 20,  720 / 20, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    raytracer.bind();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(vao);
    glDrawArrays(type, 0, 4);
} // render

void Compute::glfwEvents(SDLHelper& glfwHandler, float& mouseWheelDy, bool& running)
{

}

void Compute::printFramesToConsole(unsigned int& frameCounter,
    float& timeSinceLastUpdate, const float dt)
{
    frameCounter += 1;
    timeSinceLastUpdate += dt;
    if (frameCounter >= 60)
    {
        printf("FPS: %u\n", frameCounter);
        printf("time (us) / frame: %f\n", timeSinceLastUpdate / static_cast<float>(frameCounter));

        frameCounter = 0;
        timeSinceLastUpdate -= 1.0f;
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
