#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <stack>
#include <queue>
#include <vector>
#include <string>

struct ScoreEntry {
    int score = 0;
    int level = 0;
    std::string timestamp;

    ScoreEntry() = default;
    ScoreEntry(int s, int l, const std::string& t) : score(s), level(l), timestamp(t) {}
};

class ScoreManager {
private:
    std::stack<ScoreEntry> scoreHistory;    // For undo functionality
    std::queue<int> recentScores;          // For recent scores tracking
    std::vector<ScoreEntry> highScores;    // For persistent high scores
    int currentScore;
    int currentLevel;
    static const int MAX_HIGH_SCORES = 10;
    static const int MAX_RECENT_SCORES = 5;

public:
    ScoreManager();
    void addScore(int points);
    void setLevel(int level);
    void gameOver();
    void reset();
    int getCurrentScore() const { return currentScore; }
    int getCurrentLevel() const { return currentLevel; }
    std::vector<ScoreEntry> getHighScores() const { return highScores; }
    std::vector<int> getRecentScores() const;
    bool canUndo() const;
    void undoLastScore();
    std::string getCurrentTimestamp() const;
};

#endif