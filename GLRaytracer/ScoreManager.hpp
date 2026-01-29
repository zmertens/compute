#ifndef SCOREMANAGER_HPP
#define SCOREMANAGER_HPP

#include <cstdint>
#include <unordered_map>
#include <string>

class ScoreManager
{
public:
    ScoreManager();
    
    // Score tracking
    void addScore(uint32_t playerId, uint32_t points);
    void setScore(uint32_t playerId, uint32_t score);
    uint32_t getScore(uint32_t playerId) const;
    void resetScore(uint32_t playerId);
    
    // Distance-based scoring
    void updateDistanceScore(uint32_t playerId, float distance);
    
    // Obstacle-based scoring
    void addObstacleAvoidedBonus(uint32_t playerId, uint32_t bonus = 10);
    
    // High score management
    uint32_t getHighScore() const;
    void saveHighScore();
    void loadHighScore();
    
    // Get all scores for display
    const std::unordered_map<uint32_t, uint32_t>& getAllScores() const;
    
private:
    std::unordered_map<uint32_t, uint32_t> mPlayerScores;
    uint32_t mHighScore;
    const std::string mHighScoreFile;
};

#endif // SCOREMANAGER_HPP
