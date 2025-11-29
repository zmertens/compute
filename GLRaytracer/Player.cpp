#include "Player.hpp"

#include <algorithm>

#include "SDLHelper.hpp"
#include "Camera.hpp"

const float Player::scMouseSensitivity = 1.0f;
float Player::scMovementScalar = 220.0f;

/**
 * @brief Player::Player
 * @param camera
 * @param level
 */
Player::Player(Camera& camera)
: cPlayerSize(0.2f)
, mFirstPersonCamera(camera)
, mStartPosition(camera.getPosition())
, mMovementDir(glm::vec3(0))
, mMouseLocked(false)
, mCollisions(true)
, mInvincible(false)
, mSpeed(false)
, mInfAmmo(false)
, mStrength(false)
{

}

/**
 * @brief Player::getPosition
 * @return
 */
glm::vec3 Player::getPosition() const
{
    return mFirstPersonCamera.getPosition();
}

/**
 * @brief Player::setPosition
 * @param position
 */
void Player::setPosition(const glm::vec3& position)
{
    mFirstPersonCamera.setPosition(position);
}

/**
 * @brief Player::move
 * @param vel
 * @param dt
 */
void Player::move(const glm::vec3& vel, float dt)
{
    mFirstPersonCamera.move(vel, dt);
}

/**
 * @brief Player::input
 * @param sdlHandler
 * @param mouseWheelDelta
 * @param coords
 */
void Player::input(const SDLHelper& sdlHandler, const float mouseWheelDelta,
    const glm::vec2& coords)
{
    glm::vec2 winCenter = glm::vec2(
        static_cast<float>(SDLHelper::SDL_WINDOW_WIDTH) * 0.5f,
        static_cast<float>(SDLHelper::SDL_WINDOW_HEIGHT) * 0.5f);

    const auto& inputs = sdlHandler.getKeys();

    // Mouse lock
    if (inputs[SDL_SCANCODE_TAB]) {
        setMouseLocked(!getMouseLocked());
    }

    // keyboard movements
    if (inputs[SDL_SCANCODE_W])
    {
        mMovementDir += mFirstPersonCamera.getTarget();
    }

    if (inputs[SDL_SCANCODE_S])
    {
        mMovementDir -= mFirstPersonCamera.getTarget();
    }

    if (inputs[SDL_SCANCODE_A])
    {
        mMovementDir -= mFirstPersonCamera.getRight();
    }

    if (inputs[SDL_SCANCODE_D])
    {
        mMovementDir += mFirstPersonCamera.getRight();
    }

    // mouse wheel events
    if (mouseWheelDelta != 0)
        mFirstPersonCamera.updateFieldOfView(mouseWheelDelta);

    // rotations (mouse movements)
    if (mMouseLocked)
    {
        float xOffset = coords.x - winCenter.x;
        float yOffset = winCenter.y - coords.y;

        if (xOffset || yOffset)
        {
            mFirstPersonCamera.rotate(xOffset * scMouseSensitivity, yOffset * scMouseSensitivity, false, false);
            const_cast<SDLHelper&>(sdlHandler).setCursorPos(winCenter.x, winCenter.y);
        }
    }
}

/**
 * @brief Player::update
 * @param dt
 * @param timeSinceInit
 */
void Player::update(const float dt, const double timeSinceInit)
{
    if (glm::length(mMovementDir) > 0)
    {
        // if (mCollisions)
        // {
           glm::vec3 origin (getPosition());
           // R(t) = P + Vt
           glm::vec3 direction (origin + glm::normalize(mMovementDir * scMovementScalar * dt));
        //    glm::vec3 collision (iterateThruSpace(mLevel.getEmptySpace(), mLevel.getTileScalar(), origin, direction));
        //    mMovementDir *= collision;
           mMovementDir.y = 0.0f;
        // }

        mFirstPersonCamera.move(mMovementDir, scMovementScalar * dt);

        // reset movement direction every iteration
        mMovementDir = glm::vec3(0);
    }
    // else no movement
}

/**
 * In first person, the player's hands are rendered.
 * In third person, the entire player is rendered.
 * @brief Player::render
 */
void Player::render() const
{

}

/**
 * @brief Player::getCamera
 * @return
 */
Camera& Player::getCamera() const
{
    return mFirstPersonCamera;
}

/**
 * @brief Player::getPlayerSize
 * @return
 */
glm::vec2 Player::getPlayerSize() const
{
    return cPlayerSize;
}

/**
 * @brief Player::getMouseLocked
 * @return
 */
bool Player::getMouseLocked() const
{
    return mMouseLocked;
}

/**
 * @brief Player::setMouseLocked
 * @param mouseLocked
 */
void Player::setMouseLocked(bool mouseLocked)
{
    mMouseLocked = mouseLocked;
}

bool Player::getCollisions() const
{
    return mCollisions;
}

void Player::setCollisions(bool collisions)
{
    mCollisions = collisions;
}

bool Player::getInvincible() const
{
    return mInvincible;
}

void Player::setInvincible(bool invincible)
{
    mInvincible = invincible;
}

bool Player::getSpeed() const
{
    return mSpeed;
}

void Player::setSpeed(bool speed)
{
    mSpeed = speed;
}

bool Player::getInfAmmo() const
{
    return mInfAmmo;
}

void Player::setInfAmmo(bool infAmmo)
{
    mInfAmmo = infAmmo;
}

bool Player::getStrength() const
{
    return mStrength;
}

void Player::setStrength(bool strength)
{
    mStrength = strength;
}

/**
 * @brief Player::iterateThruSpace -- tiles?
 * @param emptySpaces
 * @param spaceScalar
 * @param origin
 * @param dir
 * @return
 */
glm::vec3 Player::iterateThruSpace(const std::vector<glm::vec3>& emptySpaces,
    const glm::vec3& spaceScalar,
    const glm::vec3& origin,
    const glm::vec3& dir) const
{
    glm::vec3 collisionVec (1);
    for (auto& emptiness : emptySpaces)
    {
        collisionVec *= rectangularCollision(origin, dir,
            glm::vec3(cPlayerSize.x, 0, cPlayerSize.y), emptiness, spaceScalar);
    }

    return collisionVec;
}

/**
 * Returns zero vector if no movement whatsoever,
 * else it returns 1 along the axis of movement.
 * @brief Player::rectangularCollision
 * @param origin
 * @param dir
 * @param objSize
 * @param rectangle
 * @param scalar
 * @return
 */
glm::vec3 Player::rectangularCollision(const glm::vec3& origin,
    const glm::vec3& dir, const glm::vec3& objSize,
    const glm::vec3& rectangle,
    const glm::vec3& scalar) const
{
    glm::vec3 result (0.0f, 1.0f, 0.0f);

    if (dir.x + objSize.x < rectangle.x * scalar.x ||
       dir.x - objSize.x > (rectangle.x + 1.0f) * scalar.x  ||
       origin.z + objSize.z < rectangle.z * scalar.z ||
       origin.z - objSize.z > (rectangle.z + 1.0f) * scalar.z)
    {
        result.x = 1.0f;
    }

    if (origin.x + objSize.x < rectangle.x * scalar.x  ||
       origin.x - objSize.x > (rectangle.x + 1.0f) * scalar.x  ||
       dir.z + objSize.z < rectangle.z * scalar.z ||
       dir.z - objSize.z > (rectangle.z + 1.0f) * scalar.z)
    {
        result.z = 1.0f;
    }

    return result;
}

/**
 * Check if the length of the distance from the player
 * to the point is equal to half the size of the sprite.
 * @brief Player::isOnExitPoint
 * @param origin
 * @return
 */
bool Player::isOnExitPoint(const glm::vec3& origin) const
{
//    const auto& exitPoints = mLevel.getExitPoints();

//    auto exited = std::find_if(exitPoints.begin(), exitPoints.end(),
//        [&] (const glm::vec3& point)->bool {
//            return glm::length(point - origin) < mLevel.getSpriteHalfWidth();
//    });

//    return (exited != exitPoints.end());
    return false; // Placeholder until level system is implemented
}

/**
 * @brief Player::isOnSpeedPowerUp
 * @param origin
 * @return
 */
bool Player::isOnSpeedPowerUp(const glm::vec3& origin) const
{
    return false;
}

/**
 * @brief Player::isOnRechargePowerUp
 * @param origin
 * @return
 */
bool Player::isOnRechargePowerUp(const glm::vec3& origin) const
{
    return false;
}

/**
 * @brief Player::isOnInvinciblePowerUp
 * @param origin
 * @return
 */
bool Player::isOnInvinciblePowerUp(const glm::vec3& origin) const
{
    return false;
}
