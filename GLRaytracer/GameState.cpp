#include "GameState.hpp"
#include <algorithm>

GameState::GameState()
    : mGameMode(GameMode::SINGLE_PLAYER)
    , mStatus(GameStatus::PLAYING)
    , mLocalPlayerId(0)
    , mForwardSpeed(30.0f)  // Default forward speed
    , mDistanceTraveled(0.0f)
{
}

GameMode GameState::getGameMode() const
{
    return mGameMode;
}

void GameState::setGameMode(GameMode mode)
{
    mGameMode = mode;
}

GameStatus GameState::getStatus() const
{
    return mStatus;
}

void GameState::setStatus(GameStatus status)
{
    mStatus = status;
}

uint32_t GameState::getLocalPlayerId() const
{
    return mLocalPlayerId;
}

void GameState::setLocalPlayerId(uint32_t id)
{
    mLocalPlayerId = id;
}

void GameState::addRemotePlayer(const RemotePlayer& player)
{
    mRemotePlayers.push_back(player);
}

void GameState::updateRemotePlayer(uint32_t playerId, const glm::vec3& position,
                                   const glm::vec3& velocity, uint32_t score)
{
    auto it = std::find_if(mRemotePlayers.begin(), mRemotePlayers.end(),
        [playerId](const RemotePlayer& player) { return player.playerId == playerId; });
    
    if (it != mRemotePlayers.end())
    {
        it->position = position;
        it->velocity = velocity;
        it->score = score;
    }
}

void GameState::removeRemotePlayer(uint32_t playerId)
{
    mRemotePlayers.erase(
        std::remove_if(mRemotePlayers.begin(), mRemotePlayers.end(),
            [playerId](const RemotePlayer& player) { return player.playerId == playerId; }),
        mRemotePlayers.end()
    );
}

const std::vector<RemotePlayer>& GameState::getRemotePlayers() const
{
    return mRemotePlayers;
}

float GameState::getForwardSpeed() const
{
    return mForwardSpeed;
}

void GameState::setForwardSpeed(float speed)
{
    mForwardSpeed = speed;
}

float GameState::getDistanceTraveled() const
{
    return mDistanceTraveled;
}

void GameState::addDistanceTraveled(float distance)
{
    mDistanceTraveled += distance;
}

void GameState::resetDistanceTraveled()
{
    mDistanceTraveled = 0.0f;
}
