#include "Compute.hpp"

#include <SDL3/SDL.h>

#include <glad/glad.h>

#define TOTAL_SPHERES 200
#define TOTAL_LIGHTS 5

const glm::vec3 Compute::CLEAR_COLOR = glm::vec3(0.f);
std::unordered_map<std::uint8_t, bool> Compute::mKepMap;


Compute::Compute()
    : mCamera(glm::vec3(0.0f, 50.0f, 200.0f), -90.0f, -10.0f, 65.0f, 0.1f, 500.0f)
      , mPlayer(mCamera)
      , mCurrentBatch(0)
      , mSamplesPerBatch(4)
      , mTotalBatches(250)
      , mUsePathTracer(true)  // Set to true to use new path tracer, false for legacy
      , mLastCameraPosition(mCamera.getPosition())
      , mLastCameraYaw(mCamera.getYaw())
      , mLastCameraPitch(mCamera.getPitch())
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
    if (mUsePathTracer) {
        computeShader.compileAndAttachShader(ShaderTypes::COMPUTE_SHADER, "./shaders/pathtracer.cs.glsl");
        SDL_Log("Using PBR Path Tracer");
    } else {
        computeShader.compileAndAttachShader(ShaderTypes::COMPUTE_SHADER, "./shaders/raytracer.cs.glsl");
        SDL_Log("Using Legacy Raytracer");
    }
    computeShader.linkProgram();
    computeShader.bind();

    std::vector<Light> lights;
    std::vector<Sphere> spheres;
    Plane plane;

    GLuint vao;
    GLuint screenTex;      // For legacy raytracer
    GLuint accumTex;       // For path tracer accumulation
    GLuint displayTex;     // For path tracer display
    GLuint shapeSSBO;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Create textures based on mode
    if (mUsePathTracer) {
        // Create accumulation texture
        glGenTextures(1, &accumTex);
        glBindTexture(GL_TEXTURE_2D, accumTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,
                       static_cast<GLsizei>(SDLHelper::SDL_WINDOW_WIDTH),
                       static_cast<GLsizei>(SDLHelper::SDL_WINDOW_HEIGHT));

        // Create display texture
        glGenTextures(1, &displayTex);
        glBindTexture(GL_TEXTURE_2D, displayTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,
                       static_cast<GLsizei>(SDLHelper::SDL_WINDOW_WIDTH),
                       static_cast<GLsizei>(SDLHelper::SDL_WINDOW_HEIGHT));

        screenTex = displayTex;  // For cleanup
    } else {
        // Legacy single texture
        glGenTextures(1, &screenTex);
        glBindTexture(GL_TEXTURE_2D, screenTex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F,
                       static_cast<GLsizei>(SDLHelper::SDL_WINDOW_WIDTH),
                       static_cast<GLsizei>(SDLHelper::SDL_WINDOW_HEIGHT));
        glBindImageTexture(0, screenTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &shapeSSBO);

    if (mUsePathTracer) {
        initPathTracer(computeShader, shapeSSBO, spheres);
    } else {
        initCompute(computeShader, shapeSSBO, spheres, plane, lights);
    }

    // Display control instructions
    SDL_Log("\n=== CONTROLS ===");
    SDL_Log("Movement: WASD or Arrow Keys");
    SDL_Log("Camera Rotation: Hold Left Mouse Button and move mouse");
    SDL_Log("Alternative: Press TAB to toggle mouse lock");
    SDL_Log("FOV Adjustment: Mouse Wheel");
    SDL_Log("Exit: ESC or close window");
    SDL_Log("================\n");

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

        float ar = static_cast<float>(SDLHelper::SDL_WINDOW_WIDTH) / static_cast<float>(SDLHelper::SDL_WINDOW_HEIGHT);

        if (mUsePathTracer) {
            renderPathTracer(computeShader, tracerShader, ar, vao, accumTex, displayTex);
        } else {
            render(computeShader, tracerShader, spheres, ar, vao, screenTex);
        }

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

void Compute::initPathTracer(Shader& pathtracer, GLuint shapeSSBO, std::vector<Sphere>& spheres)
{
    pathtracer.bind();

    SDL_Log("Initializing PBR Path Tracer...");

    // Clear existing spheres for fresh start
    spheres.clear();

    // Create interesting scene with different material types
    // Ground sphere (large Lambertian)
    spheres.emplace_back(
        glm::vec3(0.0f, -1000.0f, 0.0f),  // center
        1000.0f,                           // radius
        glm::vec3(0.5f, 0.5f, 0.5f),      // color (gray)
        MaterialType::LAMBERTIAN,          // type
        0.0f,                              // fuzz (unused for lambertian)
        0.0f                               // refractive index (unused)
    );

    // Center glass sphere
    spheres.emplace_back(
        glm::vec3(0.0f, 1.0f, 0.0f),
        1.0f,
        glm::vec3(1.0f, 1.0f, 1.0f),
        MaterialType::DIELECTRIC,
        0.0f,
        1.5f  // Glass refractive index
    );

    // Left diffuse sphere
    spheres.emplace_back(
        glm::vec3(-4.0f, 1.0f, 0.0f),
        1.0f,
        glm::vec3(0.4f, 0.2f, 0.1f),
        MaterialType::LAMBERTIAN,
        0.0f,
        0.0f
    );

    // Right metal sphere
    spheres.emplace_back(
        glm::vec3(4.0f, 1.0f, 0.0f),
        1.0f,
        glm::vec3(0.7f, 0.6f, 0.5f),
        MaterialType::METAL,
        0.0f,  // No fuzz for sharp reflection
        0.0f
    );

    // Generate random small spheres in a circle pattern
    float imgCircleRadius = 125.0f;
    float offset = 15.25f;

    for (unsigned int index = 4; index < TOTAL_SPHERES; ++index)
    {
        float matChoice = Utils::getRandomFloat(0.0f, 1.0f);

        float angle = static_cast<float>(index - 4) / static_cast<float>(TOTAL_SPHERES - 4) * 360.0f;
        float angleRad = glm::radians(angle);
        float displacement = Utils::getRandomFloat(-offset, offset);

        float xpos = glm::sin(angleRad) * imgCircleRadius + displacement;
        displacement = Utils::getRandomFloat(-offset, offset);
        float y = std::abs(displacement) * 7.5f;
        displacement = Utils::getRandomFloat(-offset, offset);
        float z = glm::cos(angleRad) * imgCircleRadius + displacement;

        glm::vec3 center = glm::vec3(xpos, y, z);
        float radius = Utils::getRandomFloat(5.0f, 12.0f);

        if (matChoice < 0.7f) {
            // Lambertian (diffuse)
            glm::vec3 albedo(
                Utils::getRandomFloat(0.1f, 0.9f),
                Utils::getRandomFloat(0.1f, 0.9f),
                Utils::getRandomFloat(0.1f, 0.9f)
            );
            spheres.emplace_back(center, radius, albedo, MaterialType::LAMBERTIAN, 0.0f, 0.0f);
        } else if (matChoice < 0.9f) {
            // Metal
            glm::vec3 albedo(
                Utils::getRandomFloat(0.5f, 1.0f),
                Utils::getRandomFloat(0.5f, 1.0f),
                Utils::getRandomFloat(0.5f, 1.0f)
            );
            float fuzz = Utils::getRandomFloat(0.0f, 0.5f);
            spheres.emplace_back(center, radius, albedo, MaterialType::METAL, fuzz, 0.0f);
        } else {
            // Glass (dielectric)
            spheres.emplace_back(center, radius, glm::vec3(1.0f), MaterialType::DIELECTRIC, 0.0f, 1.5f);
        }
    }

    // Upload sphere data to SSBO
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, shapeSSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, spheres.size() * sizeof(Sphere), spheres.data(), GL_STATIC_DRAW);

    SDL_Log("Path tracer initialized with %zu spheres", spheres.size());

#if defined(DEBUG_COMPUTE)
    std::ofstream out;
    out.open("./pathtracer_spheres.txt");
    out << "Path Tracer Sphere Data:\n";
    for (unsigned int i = 0; i < spheres.size(); ++i)
    {
        out << "Sphere[" << i << "]: center(" << spheres[i].center.x << ", "
            << spheres[i].center.y << ", " << spheres[i].center.z << ")"
            << ", radius=" << spheres[i].radius
            << ", type=" << spheres[i].materialType
            << ", albedo=(" << spheres[i].albedo.r << "," << spheres[i].albedo.g << "," << spheres[i].albedo.b << ")"
            << "\n";
    }
    out.close();
    SDL_Log("Path tracer sphere data written to pathtracer_spheres.txt");
#endif
}

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

void Compute::renderPathTracer(Shader& pathtracer, Shader& displayShader, float ar,
                                 GLuint vao, GLuint accumTex, GLuint displayTex)
{
    // Clear screen
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Check if camera has moved - if so, reset accumulation
    glm::vec3 currentPos = mCamera.getPosition();
    float currentYaw = mCamera.getYaw();
    float currentPitch = mCamera.getPitch();

    const float posEpsilon = 0.01f;
    const float angleEpsilon = 0.1f;

    if (glm::distance(currentPos, mLastCameraPosition) > posEpsilon ||
        std::abs(currentYaw - mLastCameraYaw) > angleEpsilon ||
        std::abs(currentPitch - mLastCameraPitch) > angleEpsilon)
    {
        // Camera moved - reset accumulation
        mCurrentBatch = 0;
        mLastCameraPosition = currentPos;
        mLastCameraYaw = currentYaw;
        mLastCameraPitch = currentPitch;

        // Clear accumulation texture (OpenGL 4.3 compatible method)
        // The shader will handle clearing by checking if uBatch == 0
        // No need to explicitly clear the texture
    }

    // Only compute if we haven't finished all batches
    if (mCurrentBatch < mTotalBatches)
    {
        pathtracer.bind();

        // Check for OpenGL errors after shader bind
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            SDL_Log("OpenGL error after shader bind: 0x%x", err);
        }

        // Set camera uniforms
        pathtracer.setUniform("uCamera.eye", mCamera.getPosition());
        pathtracer.setUniform("uCamera.far", mCamera.getFar());
        pathtracer.setUniform("uCamera.ray00", mCamera.getFrustumEyeRay(ar, -1, -1));
        pathtracer.setUniform("uCamera.ray01", mCamera.getFrustumEyeRay(ar, -1, 1));
        pathtracer.setUniform("uCamera.ray10", mCamera.getFrustumEyeRay(ar, 1, -1));
        pathtracer.setUniform("uCamera.ray11", mCamera.getFrustumEyeRay(ar, 1, 1));

        // Set batch uniforms
        pathtracer.setUniform("uBatch", mCurrentBatch);
        pathtracer.setUniform("uSamplesPerBatch", mSamplesPerBatch);

        // Bind textures
        glBindImageTexture(0, accumTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
        glBindImageTexture(1, displayTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

        // Dispatch compute shader - use actual window dimensions
        glDispatchCompute(
            (SDLHelper::SDL_WINDOW_WIDTH + 19) / 20,   // Ceiling division for width
            (SDLHelper::SDL_WINDOW_HEIGHT + 19) / 20,  // Ceiling division for height
            1
        );
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // Check for errors after compute dispatch
        err = glGetError();
        if (err != GL_NO_ERROR) {
            SDL_Log("OpenGL error after compute dispatch: 0x%x", err);
        }

        mCurrentBatch++;

        // Log progress
        if (mCurrentBatch % 10 == 0 || mCurrentBatch == mTotalBatches) {
            uint32_t totalSamples = mCurrentBatch * mSamplesPerBatch;
            float progress = (float)mCurrentBatch / (float)mTotalBatches * 100.0f;
            SDL_Log("Path tracing progress: %.1f%% (%u/%u batches, %u samples)",
                    progress, mCurrentBatch, mTotalBatches, totalSamples);
        }
    }

    // Display the current result
    displayShader.bind();
    displayShader.setUniform("uTexture2D", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, displayTex);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

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
