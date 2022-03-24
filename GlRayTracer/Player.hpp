#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <cstdint>

#include "./extlibs/glm/glm.hpp"

class GlfwHandler;
class Camera;

class Player final
{
public:
    Player(Camera& camera);

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3& position);
    void move(const glm::vec3& vel, float dt);
    void input(const GlfwHandler& glfwHandler, const float mouseWheelDelta,
        const glm::vec2& coords);
    void update(const float dt, const double timeSinceInit);
    void render() const;
    Camera& getCamera() const;

    glm::vec2 getPlayerSize() const;

    bool getMouseLocked() const;
    void setMouseLocked(bool mouseLocked);

    bool getCollisions() const;
    void setCollisions(bool collisions);

    bool getInvincible() const;
    void setInvincible(bool invincible);

    bool getSpeed() const;
    void setSpeed(bool speed);

    bool getInfAmmo() const;
    void setInfAmmo(bool infAmmo);

    bool getStrength() const;
    void setStrength(bool strength);

private:
    static const float scMouseSensitivity;
    static float scMovementScalar;
    const glm::vec2 cPlayerSize;
    Camera& mFirstPersonCamera;
    glm::vec3 mStartPosition;
    glm::vec3 mMovementDir;
    bool mMouseLocked;
    bool mCollisions;
    bool mInvincible;
    bool mSpeed;
    bool mInfAmmo;
    bool mStrength;
private:
    glm::vec3 iterateThruSpace(const std::vector<glm::vec3>& emptySpaces,
        const glm::vec3& spaceScalar, const glm::vec3& origin,
        const glm::vec3& dir) const;
    glm::vec3 rectangularCollision(const glm::vec3& origin,
        const glm::vec3& dir, const glm::vec3& objSize,
        const glm::vec3& rectangle,
        const glm::vec3& scalar) const;
    bool isOnExitPoint(const glm::vec3& origin) const;
    bool isOnSpeedPowerUp(const glm::vec3& origin) const;
    bool isOnRechargePowerUp(const glm::vec3& origin) const;
    bool isOnInvinciblePowerUp(const glm::vec3& origin) const;
};

#endif // PLAYER_HPP
