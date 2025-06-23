#include "ScoreManager.h"
#include <algorithm>
#include <ctime>
#include <sstream>
#include <iomanip>
 
ScoreManager::ScoreManager() : currentScore(0), currentLevel(1) {}

void ScoreManager::addScore(int points) {
    scoreHistory.push(ScoreEntry(currentScore, currentLevel, getCurrentTimestamp()));
    currentScore += points;
    
    // Add to recent scores queue
    recentScores.push(points);
    if (recentScores.size() > MAX_RECENT_SCORES) {
        recentScores.pop();
    }
}

void ScoreManager::setLevel(int level) {
    currentLevel = level;
}

void ScoreManager::gameOver() {
    // Add final score to high scores
    ScoreEntry finalScore(currentScore, currentLevel, getCurrentTimestamp());
    highScores.push_back(finalScore);
    
    // Sort high scores in descending order
    std::sort(highScores.begin(), highScores.end(),
              [](const ScoreEntry& a, const ScoreEntry& b) {
                  return a.score > b.score;
              });
    
    // Keep only top scores
    if (highScores.size() > MAX_HIGH_SCORES) {
        highScores.resize(MAX_HIGH_SCORES);
    }
}

void ScoreManager::reset() {
    currentScore = 0;
    currentLevel = 1;
    
    // Clear score history and recent scores
    while (!scoreHistory.empty()) scoreHistory.pop();
    while (!recentScores.empty()) recentScores.pop();
}

std::vector<int> ScoreManager::getRecentScores() const {
    std::vector<int> recent;
    std::queue<int> temp = recentScores;
    
    while (!temp.empty()) {
        recent.push_back(temp.front());
        temp.pop();
    }
    
    return recent;
}

bool ScoreManager::canUndo() const {
    return !scoreHistory.empty();
}

void ScoreManager::undoLastScore() {
    if (!scoreHistory.empty()) {
        ScoreEntry lastEntry = scoreHistory.top();
        scoreHistory.pop();
        currentScore = lastEntry.score;
        currentLevel = lastEntry.level;
        
        // Remove from recent scores (simplified - just pop the last one)
        if (!recentScores.empty()) {
            // This is a simplified undo - in a real implementation,
            // you might want to track what was undone more carefully
        }
    }
}

std::string ScoreManager::getCurrentTimestamp() const {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}