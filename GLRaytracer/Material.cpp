#include "Material.hpp"

/**
 * @brief Material::Material
 */
Material::Material()
: mAmbient(glm::vec3(0))
, mDiffuse(glm::vec3(0))
, mSpecular(glm::vec3(0))
, mShininess(0)
, mReflectivity(0)
, mRefractivity(0)
, mAlbedo(glm::vec3(0.5f))
, mType(MaterialType::LAMBERTIAN)
, mFuzz(0.0f)
, mRefractiveIndex(1.5f)
{

}

/**
 * @brief Material::Material
 * @param ambient
 * @param diffuse
 * @param specular
 * @param shininess
 */
Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
    float shininess)
: mAmbient(ambient)
, mDiffuse(diffuse)
, mSpecular(specular)
, mShininess(shininess)
, mReflectivity(0)
, mRefractivity(0)
, mAlbedo(diffuse)
, mType(MaterialType::LAMBERTIAN)
, mFuzz(0.0f)
, mRefractiveIndex(1.5f)
{

}

/**
 * @brief Material::Material
 * @param ambient
 * @param diffuse
 * @param specular
 * @param shininess
 * @param reflectValue
 * @param refractValue
 */
Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
    float shininess, float reflectValue, float refractValue)
: mAmbient(ambient)
, mDiffuse(diffuse)
, mSpecular(specular)
, mShininess(shininess)
, mReflectivity(reflectValue)
, mRefractivity(refractValue)
, mAlbedo(diffuse)
, mType(MaterialType::LAMBERTIAN)
, mFuzz(0.0f)
, mRefractiveIndex(refractValue > 0.0f ? refractValue : 1.5f)
{

}

/**
 * @brief Material::getAmbient
 * @return
 */
glm::vec3 Material::getAmbient() const
{
    return mAmbient;
}

/**
 * @brief Material::setAmbient
 * @param ambient
 */
void Material::setAmbient(const glm::vec3& ambient)
{
    mAmbient = ambient;
}

/**
 * @brief Material::getDiffuse
 * @return
 */
glm::vec3 Material::getDiffuse() const
{
    return mDiffuse;
}

/**
 * @brief Material::setDiffuse
 * @param diffuse
 */
void Material::setDiffuse(const glm::vec3& diffuse)
{
    mDiffuse = diffuse;
}

/**
 * @brief Material::getSpecular
 * @return
 */
glm::vec3 Material::getSpecular() const
{
    return mSpecular;
}

/**
 * @brief Material::setSpecular
 * @param specular
 */
void Material::setSpecular(const glm::vec3& specular)
{
    mSpecular = specular;
}

/**
 * @brief Material::getShininess
 * @return
 */
float Material::getShininess() const
{
    return mShininess;
}

/**
 * @brief Material::setShininess
 * @param shininess
 */
void Material::setShininess(float shininess)
{
    mShininess = shininess;
}

/**
 * @brief Material::getReflectivity
 * @return
 */
float Material::getReflectivity() const
{
    return mReflectivity;
}

/**
 * @brief Material::setReflectivity
 * @param reflectivity
 */
void Material::setReflectivity(float reflectivity)
{
    mReflectivity = reflectivity;
}

/**
 * @brief Material::getRefractivity
 * @return
 */
float Material::getRefractivity() const
{
    return mRefractivity;
}

/**
 * @brief Material::setRefractivity
 * @param refractivity
 */
void Material::setRefractivity(float refractivity)
{
    mRefractivity = refractivity;
}

/**
 * @brief Material::Material - PBR constructor
 * @param albedo - Base color
 * @param type - Material type (Lambertian, Metal, Dielectric)
 * @param fuzz - Fuzziness for metals (0-1)
 * @param refractiveIndex - Refractive index for dielectrics
 */
Material::Material(const glm::vec3& albedo, MaterialType type, float fuzz, float refractiveIndex)
: mAmbient(albedo)
, mDiffuse(albedo)
, mSpecular(glm::vec3(1.0f))
, mShininess(32.0f)
, mReflectivity(type == MaterialType::METAL ? 1.0f : 0.0f)
, mRefractivity(refractiveIndex)
, mAlbedo(albedo)
, mType(type)
, mFuzz(fuzz)
, mRefractiveIndex(refractiveIndex)
{

}

/**
 * @brief Material::getAlbedo
 * @return Base color for PBR
 */
glm::vec3 Material::getAlbedo() const
{
    return mAlbedo;
}

/**
 * @brief Material::setAlbedo
 * @param albedo
 */
void Material::setAlbedo(const glm::vec3& albedo)
{
    mAlbedo = albedo;
}

/**
 * @brief Material::getType
 * @return Material type
 */
MaterialType Material::getType() const
{
    return mType;
}

/**
 * @brief Material::setType
 * @param type
 */
void Material::setType(MaterialType type)
{
    mType = type;
}

/**
 * @brief Material::getFuzz
 * @return Fuzziness for metals
 */
float Material::getFuzz() const
{
    return mFuzz;
}

/**
 * @brief Material::setFuzz
 * @param fuzz
 */
void Material::setFuzz(float fuzz)
{
    mFuzz = fuzz;
}

/**
 * @brief Material::getRefractiveIndex
 * @return Refractive index for dielectrics
 */
float Material::getRefractiveIndex() const
{
    return mRefractiveIndex;
}

/**
 * @brief Material::setRefractiveIndex
 * @param index
 */
void Material::setRefractiveIndex(float index)
{
    mRefractiveIndex = index;
}

