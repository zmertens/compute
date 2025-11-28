#ifndef COMPUTE_HPP
#define COMPUTE_HPP

#include <cstdlib>
#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "Shader.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "SDLHelper.hpp"
#include "Utils.hpp"
#include "GLUtils.hpp"
#include "Light.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"

class Compute
{
private:
    Camera mCamera;
    Player mPlayer;
    static const glm::vec3 CLEAR_COLOR;
    static std::unordered_map<std::uint8_t, bool> mKepMap;

    void initCompute(Shader& compute, GLuint shapeSSBO,
        std::vector<Sphere>& spheres, Plane& plane,
        std::vector<Light>& lights);
    void input(SDLHelper& sdlHandler);
    void update(const float dt);
    void render(Shader& compute, Shader& raytracer,
        const std::vector<Sphere>& spheres, float ar,
        GLuint vao, GLuint tex, GLenum type = GL_TRIANGLE_STRIP);

    void sdlEvents(SDLHelper& sdlHandler, float& mouseWheelDy, bool& running);
    void printFramesToConsole(SDLHelper& sdlHandler, unsigned int frameCounter, float timeSinceLastUpdate) const noexcept;
    void printOpenGlInfo();
public:
    Compute();
    void run();
};

#endif // COMPUTE_HPP
