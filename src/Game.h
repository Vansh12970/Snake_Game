#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "Food.h"
#include "Graph.h"
#include "ScoreManager.h"
#include <vector>
#include <chrono>

class Game {
private:
    Snake* snake;
    Food* food;
    Graph* graph;
    ScoreManager* scoreManager;
    
    int boardWidth, boardHeight;
    int score;
    int level;
    bool gameOver;
    bool paused;
    
    std::chrono::steady_clock::time_point lastUpdate;
    int gameSpeed; // milliseconds

public:
    Game(int width = 40, int height = 20);
    ~Game();
    
    void initialize();
    void run();
    void update();
    void render();
    void handleInput();
    void checkCollisions();
    void nextLevel();
    void gameOverScreen();
    void pauseMenu();
    void showInstructions();
    char getKeyPress();
};

#endif