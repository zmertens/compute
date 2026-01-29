#include "ScoreManager.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>

ScoreManager::ScoreManager()
    : mHighScore(0)
    , mHighScoreFile("highscore.txt")
{
    loadHighScore();
}

void ScoreManager::addScore(uint32_t playerId, uint32_t points)
{
    mPlayerScores[playerId] += points;
    
    // Update high score if necessary
    if (mPlayerScores[playerId] > mHighScore)
    {
        mHighScore = mPlayerScores[playerId];
        saveHighScore();
    }
}

void ScoreManager::setScore(uint32_t playerId, uint32_t score)
{
    mPlayerScores[playerId] = score;
    
    if (score > mHighScore)
    {
        mHighScore = score;
        saveHighScore();
    }
}

uint32_t ScoreManager::getScore(uint32_t playerId) const
{
    auto it = mPlayerScores.find(playerId);
    return (it != mPlayerScores.end()) ? it->second : 0;
}

void ScoreManager::resetScore(uint32_t playerId)
{
    mPlayerScores[playerId] = 0;
}

void ScoreManager::updateDistanceScore(uint32_t playerId, float distance)
{
    // Add 1 point per 10 units of distance traveled
    uint32_t points = static_cast<uint32_t>(distance / 10.0f);
    addScore(playerId, points);
}

void ScoreManager::addObstacleAvoidedBonus(uint32_t playerId, uint32_t bonus)
{
    addScore(playerId, bonus);
}

uint32_t ScoreManager::getHighScore() const
{
    return mHighScore;
}

void ScoreManager::saveHighScore()
{
    std::ofstream file(mHighScoreFile);
    if (file.is_open())
    {
        file << mHighScore;
        file.close();
    }
}

void ScoreManager::loadHighScore()
{
    std::ifstream file(mHighScoreFile);
    if (file.is_open())
    {
        file >> mHighScore;
        file.close();
    }
    else
    {
        mHighScore = 0;
    }
}

const std::unordered_map<uint32_t, uint32_t>& ScoreManager::getAllScores() const
{
    return mPlayerScores;
}
