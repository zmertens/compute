#include "ObstacleManager.hpp"
#include "Utils.hpp"
#include <algorithm>

ObstacleManager::ObstacleManager()
    : mSpawnDistance(200.0f)
    , mDespawnDistance(50.0f)
    , mSpawnRate(1.0f)  // seconds between spawns
    , mTimeSinceLastSpawn(0.0f)
    , mNextObstacleId(0)
    , mDifficulty(1.0f)
{
}

void ObstacleManager::generateObstacles(const glm::vec3& playerPosition, float spawnDistance)
{
    // Generate initial set of obstacles ahead of player
    for (int i = 0; i < 10; ++i)
    {
        spawnObstacle(playerPosition + glm::vec3(0.0f, 0.0f, -spawnDistance - i * 50.0f));
    }
}

void ObstacleManager::update(const glm::vec3& playerPosition, float dt)
{
    mTimeSinceLastSpawn += dt;
    
    // Spawn new obstacles at regular intervals
    if (mTimeSinceLastSpawn >= mSpawnRate)
    {
        spawnObstacle(playerPosition + glm::vec3(0.0f, 0.0f, -mSpawnDistance));
        mTimeSinceLastSpawn = 0.0f;
    }
    
    // Deactivate obstacles that are too far behind the player
    for (auto& obstacle : mObstacles)
    {
        if (obstacle.active)
        {
            float distanceBehind = obstacle.position.z - playerPosition.z;
            if (distanceBehind > mDespawnDistance)
            {
                obstacle.active = false;
            }
        }
    }
    
    removeInactiveObstacles();
}

bool ObstacleManager::checkCollision(const glm::vec3& position, float radius) const
{
    for (const auto& obstacle : mObstacles)
    {
        if (obstacle.active)
        {
            float distance = glm::length(obstacle.position - position);
            if (distance < (obstacle.radius + radius))
            {
                return true;  // Collision detected
            }
        }
    }
    return false;
}

const std::vector<Obstacle>& ObstacleManager::getObstacles() const
{
    return mObstacles;
}

std::vector<Sphere> ObstacleManager::getObstaclesAsSpheres() const
{
    std::vector<Sphere> spheres;
    for (const auto& obstacle : mObstacles)
    {
        if (obstacle.active)
        {
            // Create sphere with red/orange color for obstacles
            glm::vec3 ambient(0.8f, 0.2f, 0.1f);
            glm::vec3 diffuse(0.9f, 0.3f, 0.2f);
            glm::vec3 specular(1.0f, 0.5f, 0.3f);
            float shininess = 50.0f;
            float reflectivity = 0.3f;
            
            spheres.emplace_back(obstacle.position, obstacle.radius,
                                ambient, diffuse, specular, shininess, reflectivity);
        }
    }
    return spheres;
}

void ObstacleManager::setSpawnRate(float rate)
{
    mSpawnRate = rate;
}

void ObstacleManager::setDifficulty(float difficulty)
{
    mDifficulty = difficulty;
}

void ObstacleManager::spawnObstacle(const glm::vec3& basePosition)
{
    Obstacle obstacle;
    
    // Random position around base position with some variation
    float xOffset = Utils::getRandomFloat(-30.0f, 30.0f);
    float yOffset = Utils::getRandomFloat(5.0f, 40.0f);
    float zOffset = Utils::getRandomFloat(-20.0f, 20.0f);
    
    obstacle.position = basePosition + glm::vec3(xOffset, yOffset, zOffset);
    obstacle.radius = Utils::getRandomFloat(5.0f, 10.0f) * mDifficulty;
    obstacle.active = true;
    obstacle.id = mNextObstacleId++;
    
    mObstacles.push_back(obstacle);
}

void ObstacleManager::removeInactiveObstacles()
{
    mObstacles.erase(
        std::remove_if(mObstacles.begin(), mObstacles.end(),
            [](const Obstacle& obs) { return !obs.active; }),
        mObstacles.end()
    );
}
