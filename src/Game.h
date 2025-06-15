#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Food.h"
#include "ScoreManager.h"
#include "Graph.h"

enum GameState { MENU, PLAYING, GAME_OVER, PAUSED, HIGH_SCORES };

class Game {
private:
    sf::RenderWindow window;
    Snake snake;
    Food food;
    ScoreManager scoreManager;
    Graph gameGraph;
    GameState state;
    sf::Font font;
    sf::Clock gameClock;
    sf::Time lastUpdate;
    
    static const int GRID_WIDTH = 30;
    static const int GRID_HEIGHT = 20;
    static const int CELL_SIZE = 25;
    static const int WINDOW_WIDTH = GRID_WIDTH * CELL_SIZE + 200; // Extra space for UI
    static const int WINDOW_HEIGHT = GRID_HEIGHT * CELL_SIZE + 100;
    
    float gameSpeed;
    bool gameRunning;

public:
    Game();
    void run();
    
private:
    void handleEvents();
    void update();
    void render();
    void renderMenu();
    void renderGame();
    void renderGameOver();
    void renderHighScores();
    void renderUI();
    void startNewGame();
    void pauseGame();
    void resumeGame();
    void nextLevel();
    bool loadFont();
    void renderText(const std::string& text, float x, float y, int size = 24, 
                   sf::Color color = sf::Color::White);
};

#endif