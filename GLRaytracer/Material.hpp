#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <memory>
#include <cstdint>

#include <glm/glm.hpp>

// Material types for physically-based rendering
enum class MaterialType : uint32_t {
    LAMBERTIAN = 0,  // Diffuse materials
    METAL = 1,       // Reflective materials with optional fuzz
    DIELECTRIC = 2   // Glass-like materials (refraction/reflection)
};

class Material
{
public:
    typedef std::unique_ptr<Material> Ptr;
public:
    explicit Material();
    explicit Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
        float shininess);
    explicit Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
        float shininess, float reflectValue, float refractValue);

    // New constructor for PBR materials
    explicit Material(const glm::vec3& albedo, MaterialType type, float fuzz = 0.0f, float refractiveIndex = 1.5f);

    glm::vec3 getAmbient() const;
    void setAmbient(const glm::vec3& ambient);

    glm::vec3 getDiffuse() const;
    void setDiffuse(const glm::vec3& diffuse);

    glm::vec3 getSpecular() const;
    void setSpecular(const glm::vec3& specular);

    float getShininess() const;
    void setShininess(float shininess);

    float getReflectivity() const;
    void setReflectivity(float reflectivity);

    float getRefractivity() const;
    void setRefractivity(float refractivity);

    // New PBR getters/setters
    glm::vec3 getAlbedo() const;
    void setAlbedo(const glm::vec3& albedo);

    MaterialType getType() const;
    void setType(MaterialType type);

    float getFuzz() const;
    void setFuzz(float fuzz);

    float getRefractiveIndex() const;
    void setRefractiveIndex(float index);

private:
    // Legacy properties (for backward compatibility)
    glm::vec3 mAmbient;
    glm::vec3 mDiffuse;
    glm::vec3 mSpecular;
    float mShininess;
    float mReflectivity;
    float mRefractivity;

    // New PBR properties
    glm::vec3 mAlbedo;          // Base color for PBR
    MaterialType mType;         // Material type
    float mFuzz;                // Fuzziness for metals (0-1)
    float mRefractiveIndex;     // Refractive index for dielectrics (e.g., 1.5 for glass)
};

#endif // MATERIAL_HPP
