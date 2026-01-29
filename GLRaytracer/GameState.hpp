#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

enum class GameMode
{
    SINGLE_PLAYER,
    MULTIPLAYER
};

enum class GameStatus
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

struct RemotePlayer
{
    uint32_t playerId;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 rotation;
    uint32_t score;
    uint64_t lastUpdateTime;
};

class GameState
{
public:
    GameState();
    
    // Game mode
    GameMode getGameMode() const;
    void setGameMode(GameMode mode);
    
    // Game status
    GameStatus getStatus() const;
    void setStatus(GameStatus status);
    
    // Local player
    uint32_t getLocalPlayerId() const;
    void setLocalPlayerId(uint32_t id);
    
    // Remote players
    void addRemotePlayer(const RemotePlayer& player);
    void updateRemotePlayer(uint32_t playerId, const glm::vec3& position, 
                           const glm::vec3& velocity, uint32_t score);
    void removeRemotePlayer(uint32_t playerId);
    const std::vector<RemotePlayer>& getRemotePlayers() const;
    
    // Game parameters
    float getForwardSpeed() const;
    void setForwardSpeed(float speed);
    
    float getDistanceTraveled() const;
    void addDistanceTraveled(float distance);
    void resetDistanceTraveled();
    
private:
    GameMode mGameMode;
    GameStatus mStatus;
    uint32_t mLocalPlayerId;
    std::vector<RemotePlayer> mRemotePlayers;
    float mForwardSpeed;
    float mDistanceTraveled;
};

#endif // GAMESTATE_HPP
