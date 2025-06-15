#include "ScoreManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <sstream>

ScoreManager::ScoreManager(int maxRecent) : maxRecentScores(maxRecent) {}

void ScoreManager::addScore(int score, int level) {
    std::string timestamp = getCurrentTimestamp();
    ScoreEntry entry(score, level, timestamp);
    
    // Add to history stack
    scoreHistory.push(entry);
    
    // Add to recent scores queue
    recentScores.push(entry);
    if (recentScores.size() > static_cast<size_t>(maxRecentScores)){
        recentScores.pop();
    }
    
    // Add to high scores and sort
    highScores.push_back(entry);
    std::sort(highScores.begin(), highScores.end(), 
              [](const ScoreEntry& a, const ScoreEntry& b) {
                  return a.score > b.score;
              });
    
    // Keep only top 10 high scores
    if (highScores.size() > 10) {
        highScores.reserve(10);
    }
}

void ScoreManager::displayScoreHistory() {
    std::cout << "\n=== SCORE HISTORY (Stack) ===\n";
    std::stack<ScoreEntry> temp = scoreHistory;
    int count = 0;
    
    while (!temp.empty() && count < 10) {
        ScoreEntry entry = temp.top();
        temp.pop();
        std::cout << "Score: " << entry.score 
                  << " | Level: " << entry.level 
                  << " | Time: " << entry.timestamp << std::endl;
        count++;
    }
}

void ScoreManager::displayRecentScores() {
    std::cout << "\n=== RECENT SCORES (Queue) ===\n";
    std::queue<ScoreEntry> temp = recentScores;
    
    while (!temp.empty()) {
        ScoreEntry entry = temp.front();
        temp.pop();
        std::cout << "Score: " << entry.score 
                  << " | Level: " << entry.level 
                  << " | Time: " << entry.timestamp << std::endl;
    }
}

void ScoreManager::displayHighScores() {
    std::cout << "\n=== HIGH SCORES ===\n";
    for (size_t i = 0; i < highScores.size(); i++) {
        std::cout << (i + 1) << ". Score: " << highScores[i].score 
                  << " | Level: " << highScores[i].level 
                  << " | Time: " << highScores[i].timestamp << std::endl;
    }
}

std::string ScoreManager::getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void ScoreManager::saveScoresToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& score : highScores) {
            file << score.score << "," << score.level << "," << score.timestamp << std::endl;
        }
        file.close();
    }
}

void ScoreManager::loadScoresFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string scoreStr, levelStr, timestamp;
            
            if (std::getline(iss, scoreStr, ',') && 
                std::getline(iss, levelStr, ',') && 
                std::getline(iss, timestamp)) {
                
                int score = std::stoi(scoreStr);
                int level = std::stoi(levelStr);
                highScores.push_back(ScoreEntry(score, level, timestamp));
            }
        }
        file.close();
        
        // Sort high scores
        std::sort(highScores.begin(), highScores.end(), 
                  [](const ScoreEntry& a, const ScoreEntry& b) {
                      return a.score > b.score;
                  });
    }
}