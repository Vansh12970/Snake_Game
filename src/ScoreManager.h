#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <stack>
#include <queue>
#include <vector>
#include <string>

struct ScoreEntry {
    int score;
    int level;
    std::string timestamp;
    
    // Add default constructor
    ScoreEntry() : score(0), level(0), timestamp("") {}
    
    // Your existing constructor
    ScoreEntry(int s, int l, const std::string& t) : score(s), level(l), timestamp(t) {}
};

class ScoreManager {
private:
    std::stack<ScoreEntry> scoreHistory;
    std::queue<ScoreEntry> recentScores;
    std::vector<ScoreEntry> highScores;
    int maxRecentScores;

public:
    ScoreManager(int maxRecent = 10);
    void addScore(int score, int level);
    void displayScoreHistory();
    void displayRecentScores();
    void displayHighScores();
    std::string getCurrentTimestamp();
    void saveScoresToFile(const std::string& filename);
    void loadScoresFromFile(const std::string& filename);
};

#endif