#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "Food.h"
#include "Graph.h"
#include "ScoreManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <chrono>

enum GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    INSTRUCTIONS
};

class Game {
private:
    // SFML Components
    sf::RenderWindow* window;
    sf::Font font;
    sf::Font titleFont;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    
    // Audio
    sf::SoundBuffer eatBuffer;
    sf::SoundBuffer gameOverBuffer;
    sf::Sound eatSound;
    sf::Sound gameOverSound;
    
    // Game Objects
    Snake* snake;
    Food* food;
    Graph* graph;
    ScoreManager* scoreManager;
    
    // Game Properties
    int boardWidth, boardHeight;
    int score;
    int level;
    bool gameOver;
    GameState gameState;
    
    // Timing
    std::chrono::steady_clock::time_point lastUpdate;
    int gameSpeed;
    
    // Graphics Properties
    static const int CELL_SIZE = 25;
    static const int BOARD_OFFSET_X = 50;
    static const int BOARD_OFFSET_Y = 100;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color wallColor;
    sf::Color snakeHeadColor;
    sf::Color snakeBodyColor;
    sf::Color foodColor;
    sf::Color textColor;

public:
    Game(int width = 30, int height = 20);
    ~Game();
    
    void initialize();
    void run();
    void update();
    void render();
    void handleInput();
    void checkCollisions();
    void nextLevel();
    
    // UI Rendering Functions
    void renderMenu();
    void renderGame();
    void renderPauseMenu();
    void renderGameOver();
    void renderInstructions();
    void renderUI();
    
    // Utility Functions
    void setupColors();
    void loadAssets();
    sf::Vector2f getCellPosition(int x, int y);
    void drawGradientBackground();
    void drawGlowEffect(sf::Vector2f position, sf::Color color);
};

#endif