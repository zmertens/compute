#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <glm/glm.hpp>

// the layout of the data here is pretty important!
class Sphere
{
public:
    glm::vec4 center;
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float radius;
    float radius2;
    float shininess;
    float reflectivity;

public:
    Sphere(const glm::vec3& cent, const float rad,
        const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec,
        const float shiny, const float refl)
    : center(glm::vec4(cent, 0.0))
    , ambient(glm::vec4(amb, 0.0))
    , diffuse(glm::vec4(diff, 0.0))
    , specular(glm::vec4(spec, 0.0))
    , radius(rad)
    , radius2(rad * rad)
    , shininess(shiny)
    , reflectivity(refl)
    {

    }
};

#endif // SPHERE_HPP

