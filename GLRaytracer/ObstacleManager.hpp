#ifndef OBSTACLEMANAGER_HPP
#define OBSTACLEMANAGER_HPP

#include <vector>
#include <glm/glm.hpp>
#include "Sphere.hpp"

struct Obstacle
{
    glm::vec3 position;
    float radius;
    bool active;
    uint32_t id;
};

class ObstacleManager
{
public:
    ObstacleManager();
    
    // Obstacle generation
    void generateObstacles(const glm::vec3& playerPosition, float spawnDistance);
    void update(const glm::vec3& playerPosition, float dt);
    
    // Collision detection
    bool checkCollision(const glm::vec3& position, float radius) const;
    
    // Get obstacles for rendering
    const std::vector<Obstacle>& getObstacles() const;
    std::vector<Sphere> getObstaclesAsSpheres() const;
    
    // Configuration
    void setSpawnRate(float rate);
    void setDifficulty(float difficulty);
    
private:
    std::vector<Obstacle> mObstacles;
    float mSpawnDistance;
    float mDespawnDistance;
    float mSpawnRate;
    float mTimeSinceLastSpawn;
    uint32_t mNextObstacleId;
    float mDifficulty;
    
    void spawnObstacle(const glm::vec3& playerPosition);
    void removeInactiveObstacles();
};

#endif // OBSTACLEMANAGER_HPP
