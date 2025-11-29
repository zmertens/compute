#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <glm/glm.hpp>
#include <cstdint>
#include "Material.hpp"

// the layout of the data here is pretty important for GPU alignment!
class Sphere
{
public:
    glm::vec4 center;
    glm::vec4 ambient;      // Legacy - kept for backward compatibility
    glm::vec4 diffuse;      // Legacy
    glm::vec4 specular;     // Legacy
    float radius;
    float radius2;
    float shininess;        // Legacy
    float reflectivity;     // Legacy

    // New PBR properties (must maintain 16-byte alignment)
    glm::vec4 albedo;       // RGB color + padding
    uint32_t materialType;  // 0=Lambertian, 1=Metal, 2=Dielectric
    float fuzz;             // For metals
    float refractiveIndex;  // For dielectrics
    uint32_t padding;       // Maintain alignment

public:
    // Legacy constructor
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
    , albedo(glm::vec4(diff, 0.0))
    , materialType(0)  // Lambertian by default
    , fuzz(0.0f)
    , refractiveIndex(1.5f)
    , padding(0)
    {

    }

    // New PBR constructor
    Sphere(const glm::vec3& cent, const float rad,
           const glm::vec3& color, MaterialType matType,
           float fuzziness = 0.0f, float refractIdx = 1.5f)
    : center(glm::vec4(cent, 0.0))
    , ambient(glm::vec4(color, 0.0))
    , diffuse(glm::vec4(color, 0.0))
    , specular(glm::vec4(1.0f))
    , radius(rad)
    , radius2(rad * rad)
    , shininess(32.0f)
    , reflectivity(matType == MaterialType::METAL ? 1.0f : 0.0f)
    , albedo(glm::vec4(color, 0.0))
    , materialType(static_cast<uint32_t>(matType))
    , fuzz(fuzziness)
    , refractiveIndex(refractIdx)
    , padding(0)
    {

    }
};

#endif // SPHERE_HPP

