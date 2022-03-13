#ifndef PLANE_HPP
#define PLANE_HPP

#include "./extlibs/glm/glm.hpp"

#include "Material.hpp"

class Plane
{
public:
    Material material;
    glm::vec3 point;
    glm::vec3 normal;
};

#endif // PLANE_HPP

