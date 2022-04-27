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
#include "GlfwHandler.hpp"
#include "Utils.hpp"
#include "GlUtils.hpp"
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
    void input(GlfwHandler& glfwHandler, const float mouseWheelDelta, bool& running);
    void update(const float dt, const double timeSinceInit);
    void render(Shader& compute, Shader& raytracer,
        const std::vector<Sphere>& spheres, float ar,
        GLuint vao, GLuint tex, GLenum type = GL_TRIANGLE_STRIP);

    void glfwEvents(GlfwHandler& glfwHandler, float& mouseWheelDy, bool& running);
    void printFramesToConsole(unsigned int& frameCounter, const double& timeSinceLastUpdate, const float dt);
    void printOpenGlInfo();
public:
    Compute();
    void run();
};

#endif // COMPUTE_HPP
