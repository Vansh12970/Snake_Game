#include "Game.h"
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>

Game::Game() : 
    window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game with Data Structures"),
    snake(3, GRID_HEIGHT / 2),
    food(3),
    gameGraph(GRID_WIDTH, GRID_HEIGHT),
    state(MENU),
    gameSpeed(0.15f),
    gameRunning(false) {
    
    // Background music
if (!bgMusic.openFromFile("../assets/audio/background.ogg")) {
    std::cerr << "Failed to load background music" << std::endl;
} else {
    bgMusic.setLoop(true);
    bgMusic.setVolume(10); 
    bgMusic.play();
}

// Sound effects
if (!eatBuffer.loadFromFile("../assets/audio/eat.mp3")) {
    std::cerr << "Failed to load eat sound" << std::endl;
} else{
    eatSound.setBuffer(eatBuffer);
    eatSound.setVolume(100.0f); 
}
if (!collisionBuffer.loadFromFile("../assets/audio/hit.wav")) {
    std::cerr << "Failed to load collision sound" << std::endl;
} else {
    collisionSound.setBuffer(collisionBuffer);
    collisionSound.setVolume(100.0f); 
}
if (!levelBuffer.loadFromFile("../assets/audio/levelup.wav")) {
    std::cerr << "Failed to load level up sound" << std::endl;
} else {
    levelUpSound.setBuffer(levelBuffer);
    levelUpSound.setVolume(100.0f); 
}

// Set buffers to sound players
eatSound.setBuffer(eatBuffer);
collisionSound.setBuffer(collisionBuffer);
levelUpSound.setBuffer(levelBuffer);

     if (!food.loadTexture("../assets/images/apple.png")) {
        std::cerr << "Failed to load apple texture!" << std::endl;
    }    
    if (!loadFont()) {
        std::cout << "Warning: Could not load font. Using default font." << std::endl;
    }
     if (!bgMusic.openFromFile("../assets/audio/background.ogg")) {
        std::cout << "Failed to load background music" << std::endl;
    } else {
        bgMusic.setLoop(true);
        bgMusic.play();
    }

    if (!eatBuffer.loadFromFile("../assets/audio/eat.wav"))
        std::cout << "Failed to load eat sound" << std::endl;
    if (!collisionBuffer.loadFromFile("../assets/audio/hit.wav"))
        std::cout << "Failed to load collision sound" << std::endl;
    if (!levelBuffer.loadFromFile("../assets/audio/levelup.wav"))
        std::cout << "Failed to load level up sound" << std::endl;

    eatSound.setBuffer(eatBuffer);
    collisionSound.setBuffer(collisionBuffer);
    levelUpSound.setBuffer(levelBuffer);

    
    window.setFramerateLimit(60);
    
}

bool Game::loadFont() {
    // font
    return font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||  // Windows
           font.loadFromFile("/System/Library/Fonts/Arial.ttf") ||  
           font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"); 
}

void Game::renderText(const std::string& text, float x, float y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setOutlineColor(sf::Color::Black);
    sfText.setOutlineThickness(2);
    sfText.setStyle(sf::Text::Bold);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

void Game::renderUI() {
    int uiX = GRID_WIDTH * CELL_SIZE + 10;

    sf::RectangleShape uiPanel;
    uiPanel.setSize(sf::Vector2f(190, WINDOW_HEIGHT - 20));
    uiPanel.setPosition(GRID_WIDTH * CELL_SIZE + 5, 10);
    uiPanel.setFillColor(sf::Color(25, 25, 25, 230));
    uiPanel.setOutlineColor(sf::Color::White);
    uiPanel.setOutlineThickness(2);
    window.draw(uiPanel);

    std::stringstream ss;
    ss << "Score: " << scoreManager.getCurrentScore();
    renderText(ss.str(), uiX, 20, 22, sf::Color::White);

    ss.str("");
    ss << "Level: " << scoreManager.getCurrentLevel();
    renderText(ss.str(), uiX, 50, 22, sf::Color::White);

    ss.str("");
    ss << "Length: " << snake.getLength();
    renderText(ss.str(), uiX, 80, 22, sf::Color::White);

    renderText("Recent Points:", uiX, 120, 18, sf::Color::Cyan);
    auto recentScores = scoreManager.getRecentScores();
    for (size_t i = 0; i < recentScores.size(); i++) {
        ss.str("");
        ss << "+" << recentScores[i];
        renderText(ss.str(), uiX, 145 + i * 20, 16, sf::Color::Green);
    }

    ss.str("");
    ss << "Food: " << food.getCount();
    renderText(ss.str(), uiX, 260, 22, sf::Color::Red);

    renderText("Controls:", uiX, 310, 18, sf::Color::Yellow);
    renderText("WASD: Move", uiX, 335, 14);
    renderText("P: Pause", uiX, 355, 14);
    renderText("U: Undo Score", uiX, 375, 14);
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        
        if (state == PLAYING && gameClock.getElapsedTime().asSeconds() > gameSpeed) {
            update();
            gameClock.restart();
        }
        
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            switch (state) {
                case MENU:
                    if (event.key.code == sf::Keyboard::Space) {
                        startNewGame();
                    } else if (event.key.code == sf::Keyboard::H) {
                        state = HIGH_SCORES;
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                    break;
                    
                case PLAYING:
                    if (event.key.code == sf::Keyboard::W) {
                        snake.setDirection(UP);
                    } else if (event.key.code == sf::Keyboard::S) {
                        snake.setDirection(DOWN);
                    } else if (event.key.code == sf::Keyboard::A) {
                        snake.setDirection(LEFT);
                    } else if (event.key.code == sf::Keyboard::D) {
                        snake.setDirection(RIGHT);
                    } else if (event.key.code == sf::Keyboard::P) {
                        pauseGame();
                    } else if (event.key.code == sf::Keyboard::U && scoreManager.canUndo()) {
                        scoreManager.undoLastScore();
                    }
                    break;
                    
                case PAUSED:
                    if (event.key.code == sf::Keyboard::P) {
                        resumeGame();
                    }
                    break;
                    
                case GAME_OVER:
                    if (event.key.code == sf::Keyboard::Space) {
                        state = MENU;
                    } else if (event.key.code == sf::Keyboard::R) {
                        startNewGame();
                    }
                    break;
                    
                case HIGH_SCORES:
                    if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Space) {
                        state = MENU;
                    }
                    break;
            }
        }
    }
}

void Game::update() {
    if (state != PLAYING) return;
    
    SnakeSegment head = snake.getHead();
    std::cout << "Head at: (" << head.x << ", " << head.y << ")\n";

if (!gameGraph.isValidMove(head.x, head.y, head.x, head.y)) {
    std::cout << "Invalid move at: (" << head.x << ", " << head.y << ")\n";
}
if (gameGraph.isWall(head.x, head.y)) {
    std::cout << "Wall detected at head position!\n";
}
  
    SnakeSegment newHead = head;
    
    // Calculate new head position based on direction
    switch (snake.getHead().x < head.x ? LEFT : 
            snake.getHead().x > head.x ? RIGHT :
            snake.getHead().y < head.y ? UP : DOWN) {
        //  the actual direction is handled in Snake class
    }
    
    snake.move();
    head = snake.getHead();
    
    // Check wall collision using graph
   if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT || gameGraph.isWall(head.x, head.y)) {
    scoreManager.gameOver();
    collisionSound.play();
    state = GAME_OVER;
    return;
}

    
    // Check boundary collision
    if (head.x < 0 || head.x >= GRID_WIDTH || head.y < 0 || head.y >= GRID_HEIGHT) {
        scoreManager.gameOver();
        collisionSound.play();
        state = GAME_OVER;
        return;
    }
    
    // Check self collision
    if (snake.checkSelfCollision()) {
        collisionSound.play();
        scoreManager.gameOver();
        state = GAME_OVER;
        return;
    }
    
    // Check food collision
    if (food.checkCollision(head.x, head.y)) {
        eatSound.play();
        snake.grow();
        scoreManager.addScore(10);
        
        // Check for level up
        if (scoreManager.getCurrentScore() % 100 == 0) {
            levelUpSound.play();
            nextLevel();
        }
        
        // Spawn new food
        std::vector<sf::Vector2i> occupiedPositions;
        for (const auto& segment : snake.getBody()) {
            occupiedPositions.push_back(sf::Vector2i(segment.x, segment.y));
        }
        food.spawnRandom(GRID_WIDTH, GRID_HEIGHT, occupiedPositions);
    }
    
    // Maintain food count
    if (food.getCount() < 3) {
        std::vector<sf::Vector2i> occupiedPositions;
        for (const auto& segment : snake.getBody()) {
            occupiedPositions.push_back(sf::Vector2i(segment.x, segment.y));
        }
        food.spawnRandom(GRID_WIDTH, GRID_HEIGHT, occupiedPositions);
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    
    switch (state) {
        case MENU:
            renderMenu();
            break;
        case PLAYING:
        case PAUSED:
            renderGame();
            break;
        case GAME_OVER:
            renderGameOver();
            break;
        case HIGH_SCORES:
            renderHighScores();
            break;
    }
    
    window.display();
}

void Game::renderMenu() {
    renderText("SNAKE GAME", WINDOW_WIDTH / 2 - 150, 150, 48, sf::Color::Green);
    renderText("Press SPACE to Start", WINDOW_WIDTH / 2 - 120, 250);
    renderText("Press H for High Scores", WINDOW_WIDTH / 2 - 130, 300);
    renderText("Press ESC to Exit", WINDOW_WIDTH / 2 - 100, 350);
    renderText("Controls: WASD keys, P (Pause)", WINDOW_WIDTH / 2 - 200, 450, 18);
}

void Game::renderGame() {
    sf::RectangleShape bg;
    bg.setSize(sf::Vector2f(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE));
    bg.setFillColor(sf::Color(10, 10, 10));
    window.draw(bg);

    sf::RectangleShape gridLine(sf::Vector2f(1, GRID_HEIGHT * CELL_SIZE));
    gridLine.setFillColor(sf::Color(35, 35, 35));
    for (int x = 0; x <= GRID_WIDTH; x++) {
        gridLine.setPosition(x * CELL_SIZE, 0);
        window.draw(gridLine);
    }

    gridLine.setSize(sf::Vector2f(GRID_WIDTH * CELL_SIZE, 1));
    for (int y = 0; y <= GRID_HEIGHT; y++) {
        gridLine.setPosition(0, y * CELL_SIZE);
        window.draw(gridLine);
    }

    sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    wall.setFillColor(sf::Color(80, 80, 80));
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (gameGraph.isWall(x, y)) {
                wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                window.draw(wall);
            }
        }
    }

    snake.render(window, CELL_SIZE);
    food.render(window, CELL_SIZE);

    renderUI();

    if (state == PAUSED) {
        renderText("PAUSED", WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 50, 36, sf::Color::Yellow);
        renderText("Press P to Resume", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2);
    }
}

/*
void Game::renderGame() {
    // Render grid background
    sf::RectangleShape gridLine(sf::Vector2f(1, GRID_HEIGHT * CELL_SIZE));
    gridLine.setFillColor(sf::Color(50, 50, 50));
    for (int x = 0; x <= GRID_WIDTH; x++) {
        gridLine.setPosition(x * CELL_SIZE, 0);
        window.draw(gridLine);
    }
    
    gridLine.setSize(sf::Vector2f(GRID_WIDTH * CELL_SIZE, 1));
    for (int y = 0; y <= GRID_HEIGHT; y++) {
        gridLine.setPosition(0, y * CELL_SIZE);
        window.draw(gridLine);
    }
    
    // Render walls
    sf::RectangleShape wall(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    wall.setFillColor(sf::Color(100, 100, 100));
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            if (gameGraph.isWall(x, y)) {
                wall.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                window.draw(wall);
            }
        }
    }
    
    // Render game objects
    snake.render(window, CELL_SIZE);
    food.render(window, CELL_SIZE);
    
    // Render UI
    renderUI();
    
    if (state == PAUSED) {
        renderText("PAUSED", WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 50, 36, sf::Color::Yellow);
        renderText("Press P to Resume", WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2);
    }
}*/

void Game::renderGameOver() {
    renderText("GAME OVER", WINDOW_WIDTH / 2 - 120, 200, 36, sf::Color::Red);
    
    std::stringstream ss;
    ss << "Final Score: " << scoreManager.getCurrentScore();
    renderText(ss.str(), WINDOW_WIDTH / 2 - 100, 270);
    
    ss.str("");
    ss << "Level Reached: " << scoreManager.getCurrentLevel();
    renderText(ss.str(), WINDOW_WIDTH / 2 - 100, 320);
    renderText("Press SPACE for Menu", WINDOW_WIDTH / 2 - 120, 370);
    renderText("Press R to Restart", WINDOW_WIDTH / 2 - 100, 420);
}

void Game::renderHighScores() {
    renderText("HIGH SCORES", WINDOW_WIDTH / 2 - 120, 100, 36, sf::Color::Yellow);
    
    auto highScores = scoreManager.getHighScores();
    for (size_t i = 0; i < highScores.size() && i < 10; i++) {
        std::stringstream ss;
        ss << (i + 1) << ". Score: " << highScores[i].score 
           << " Level: " << highScores[i].level;
        renderText(ss.str(), 50, 150 + i * 30, 20);
    }
    
    if (highScores.empty()) {
        renderText("No high scores yet!", WINDOW_WIDTH / 2 - 100, 200);
    }
    
    renderText("Press ESC to return to menu", WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT - 50);
}
/*
void Game::renderUI() {
    int uiX = GRID_WIDTH * CELL_SIZE + 10;
    
    // Current score
    std::stringstream ss;
    ss << "Score: " << scoreManager.getCurrentScore();
    renderText(ss.str(), uiX, 20, 20, sf::Color::White);
    
    // Current level
    ss.str("");
    ss << "Level: " << scoreManager.getCurrentLevel();
    renderText(ss.str(), uiX, 50, 20, sf::Color::White);
    
    // Snake length
    ss.str("");
    ss << "Length: " << snake.getLength();
    renderText(ss.str(), uiX, 80, 20, sf::Color::White);
    
    // Recent scores (using queue)
    renderText("Recent Points:", uiX, 120, 16, sf::Color::Cyan);
    auto recentScores = scoreManager.getRecentScores();
    for (size_t i = 0; i < recentScores.size(); i++) {
        ss.str("");
        ss << "+" << recentScores[i];
        renderText(ss.str(), uiX, 140 + i * 20, 14, sf::Color::Green);
    }
    
    // Food count
    ss.str("");
    ss << "Food: " << food.getCount();
    renderText(ss.str(), uiX, 280, 20, sf::Color::Red);
    
    // Controls
    renderText("Controls:", uiX, 320, 16, sf::Color::Yellow);
    renderText("Arrows: Move", uiX, 340, 12);
    renderText("P: Pause", uiX, 355, 12);
    renderText("U: Undo Score", uiX, 370, 12);
}
*/
void Game::startNewGame() {
    scoreManager.reset();
    gameGraph.generateWallLevel(1);
    
    int startX = 5;
    int startY = 5;
    while (gameGraph.isWall(startX, startY)) {
        startX++;
        if (startX >= GRID_WIDTH) {
            startX = 0;
            startY++;
            if (startY >= GRID_HEIGHT) startY = 0;
        }
    }
    snake.reset(startX, startY);
    std::cout << "Starting at (" << startX << ", " << startY << ")" << std::endl;

    food.clear();
    food.loadTexture("../assets/images/apple.png"); 

    
    std::vector<sf::Vector2i> occupiedPositions;
    for (const auto& segment : snake.getBody()) {
        occupiedPositions.push_back(sf::Vector2i(segment.x, segment.y));
    }
    food.spawnRandom(GRID_WIDTH, GRID_HEIGHT, occupiedPositions);
    
    state = PLAYING;
    gameRunning = true;
    gameClock.restart();
}


void Game::pauseGame() {
    state = PAUSED;
}

void Game::resumeGame() {
    state = PLAYING;
    gameClock.restart();
}

void Game::nextLevel() {
    int newLevel = scoreManager.getCurrentLevel() + 1;
    scoreManager.setLevel(newLevel);
    
    // Increase game speed
    gameSpeed = std::max(0.05f, gameSpeed - 0.01f);
    
    // Generate new wall layout
    gameGraph.generateWallLevel(newLevel);
    
    // Clear and respawn food
    food.clear();
    std::vector<sf::Vector2i> occupiedPositions;
    for (const auto& segment : snake.getBody()) {
        occupiedPositions.push_back(sf::Vector2i(segment.x, segment.y));
    }
    food.spawnRandom(GRID_WIDTH, GRID_HEIGHT, occupiedPositions);
}
/*S
void Game::renderText(const std::string& text, float x, float y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(font);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    window.draw(sfText);
}*/