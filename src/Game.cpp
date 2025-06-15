#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game(int width, int height) : boardWidth(width), boardHeight(height),
                                   score(0), level(1), gameOver(false), gameState(MENU), gameSpeed(200) {
    // Create window
    window = new sf::RenderWindow(sf::VideoMode(
        boardWidth * CELL_SIZE + BOARD_OFFSET_X * 2 + 300, 
        boardHeight * CELL_SIZE + BOARD_OFFSET_Y * 2
    ), "Snake Game - Modern Edition", sf::Style::Titlebar | sf::Style::Close);
    window->setFramerateLimit(60);
    
    // Initialize game objects
    snake = new Snake(Position(5, 5));
    food = new Food(width, height);
    graph = new Graph(width, height);
    scoreManager = new ScoreManager();
    
    // Setup graphics
    setupColors();
    loadAssets();
    
    scoreManager->loadScoresFromFile("scores.txt");
}

Game::~Game() {
    delete snake;
    delete food;
    delete graph;
    delete scoreManager;
    delete window;
}

void Game::setupColors() {
    backgroundColor = sf::Color(20, 25, 40);
    wallColor = sf::Color(100, 120, 150);
    snakeHeadColor = sf::Color(50, 255, 50);
    snakeBodyColor = sf::Color(30, 200, 30);
    foodColor = sf::Color(255, 100, 100);
    textColor = sf::Color(220, 220, 220);
}

void Game::loadAssets() {
    // Load default font (you can replace with custom font files)
    if (!font.loadFromFile("arial.ttf")) {
        // Use default system font if custom font not found
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            // Fallback - SFML will use default font
            std::cout << "Warning: Could not load custom font, using default.\n";
        }
    }
    
    titleFont = font; // Use same font for now
    
    // Create gradient background texture
    sf::Image bgImage;
    bgImage.create(window->getSize().x, window->getSize().y);
    for (unsigned int y = 0; y < window->getSize().y; y++) {
        float ratio = (float)y / window->getSize().y;
        sf::Uint8 r = 20 + (30 * ratio);
        sf::Uint8 g = 25 + (40 * ratio);
        sf::Uint8 b = 40 + (60 * ratio);
        for (unsigned int x = 0; x < window->getSize().x; x++) {
            bgImage.setPixel(x, y, sf::Color(r, g, b));
        }
    }
    backgroundTexture.loadFromImage(bgImage);
    backgroundSprite.setTexture(backgroundTexture);
}

void Game::initialize() {
    graph->generateWalls(level);
    food->spawnFood(snake->getBodyPositions(), graph->getWalls());
    lastUpdate = std::chrono::steady_clock::now();
    gameState = MENU;
}

void Game::run() {
    initialize();
    
    while (window->isOpen()) {
        handleInput();
        
        if (gameState == PLAYING) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
            
            if (elapsed.count() >= gameSpeed) {
                update();
                lastUpdate = now;
            }
        }
        
        render();
    }
}

void Game::update() {
    if (gameState != PLAYING) return;
    
    snake->move();
    checkCollisions();
    food->spawnFood(snake->getBodyPositions(), graph->getWalls());
}

void Game::render() {
    window->clear(backgroundColor);
    window->draw(backgroundSprite);
    
    switch (gameState) {
        case MENU:
            renderMenu();
            break;
        case PLAYING:
            renderGame();
            break;
        case PAUSED:
            renderGame();
            renderPauseMenu();
            break;
        case GAME_OVER:
            renderGame();
            renderGameOver();
            break;
        case INSTRUCTIONS:
            renderInstructions();
            break;
    }
    
    window->display();
}

void Game::renderMenu() {
    // Title
    sf::Text title("SNAKE GAME", titleFont, 60);
    title.setFillColor(sf::Color(100, 255, 100));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition((window->getSize().x - titleBounds.width) / 2, 100);
    window->draw(title);
    
    // Subtitle
    sf::Text subtitle("Modern Edition", font, 24);
    subtitle.setFillColor(sf::Color(150, 200, 255));
    sf::FloatRect subtitleBounds = subtitle.getLocalBounds();
    subtitle.setPosition((window->getSize().x - subtitleBounds.width) / 2, 180);
    window->draw(subtitle);
    
    // Menu options
    std::vector<std::string> options = {
        "PRESS SPACE TO START",
        "I - Instructions",
        "H - High Scores",
        "ESC - Exit"
    };
    
    for (size_t i = 0; i < options.size(); i++) {
        sf::Text option(options[i], font, 20);
        option.setFillColor(i == 0 ? sf::Color(255, 255, 100) : textColor);
        sf::FloatRect optionBounds = option.getLocalBounds();
        option.setPosition((window->getSize().x - optionBounds.width) / 2, 280 + i * 40);
        window->draw(option);
    }
    
    // Animated elements
    static float time = 0;
    time += 0.1f;
    
    // Pulsing start text
    sf::Text startText("PRESS SPACE TO START", font, 24);
    float alpha = 128 + 127 * sin(time);
    startText.setFillColor(sf::Color(255, 255, 100, (sf::Uint8)alpha));
    sf::FloatRect startBounds = startText.getLocalBounds();
    startText.setPosition((window->getSize().x - startBounds.width) / 2, 450);
    window->draw(startText);
}

void Game::renderGame() {
    // Draw board background
    sf::RectangleShape boardBg(sf::Vector2f(boardWidth * CELL_SIZE, boardHeight * CELL_SIZE));
    boardBg.setPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y);
    boardBg.setFillColor(sf::Color(30, 35, 50, 150));
    boardBg.setOutlineThickness(2);
    boardBg.setOutlineColor(sf::Color(100, 120, 150));
    window->draw(boardBg);
    
    // Draw walls with glow effect
    auto walls = graph->getWalls();
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            if (walls[y][x] == 1) {
                sf::Vector2f pos = getCellPosition(x, y);
                
                // Glow effect
                drawGlowEffect(pos, wallColor);
                
                // Wall block
                sf::RectangleShape wall(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
                wall.setPosition(pos.x + 1, pos.y + 1);
                wall.setFillColor(wallColor);
                window->draw(wall);
            }
        }
    }
    
    // Draw food with glow effect
    auto foodPositions = food->getFoodPositions();
    for (const auto& foodPos : foodPositions) {
        sf::Vector2f pos = getCellPosition(foodPos.x, foodPos.y);
        
        // Glow effect
        drawGlowEffect(pos, foodColor);
        
        // Food circle
        sf::CircleShape foodShape(CELL_SIZE / 2 - 3);
        foodShape.setPosition(pos.x + 3, pos.y + 3);
        foodShape.setFillColor(foodColor);
        window->draw(foodShape);
        
        // Inner highlight
        sf::CircleShape highlight(CELL_SIZE / 4);
        highlight.setPosition(pos.x + CELL_SIZE/4, pos.y + CELL_SIZE/4);
        highlight.setFillColor(sf::Color(255, 200, 200, 100));
        window->draw(highlight);
    }
    
    // Draw snake with glow effect
    auto snakeBody = snake->getBodyPositions();
    for (size_t i = 0; i < snakeBody.size(); i++) {
        sf::Vector2f pos = getCellPosition(snakeBody[i].x, snakeBody[i].y);
        
        if (i == 0) { // Head
            drawGlowEffect(pos, snakeHeadColor);
            
            sf::CircleShape head(CELL_SIZE / 2 - 2);
            head.setPosition(pos.x + 2, pos.y + 2);
            head.setFillColor(snakeHeadColor);
            window->draw(head);
            
            // Eyes
            sf::CircleShape eye1(2);
            sf::CircleShape eye2(2);
            eye1.setFillColor(sf::Color::Black);
            eye2.setFillColor(sf::Color::Black);
            eye1.setPosition(pos.x + 8, pos.y + 8);
            eye2.setPosition(pos.x + 15, pos.y + 8);
            window->draw(eye1);
            window->draw(eye2);
        } else { // Body
            sf::RectangleShape body(sf::Vector2f(CELL_SIZE - 4, CELL_SIZE - 4));
            body.setPosition(pos.x + 2, pos.y + 2);
            body.setFillColor(snakeBodyColor);
            window->draw(body);
        }
    }
    
    renderUI();
}

void Game::renderUI() {
    // Score panel
    sf::RectangleShape panel(sf::Vector2f(280, 200));
    panel.setPosition(boardWidth * CELL_SIZE + BOARD_OFFSET_X + 20, BOARD_OFFSET_Y);
    panel.setFillColor(sf::Color(40, 45, 60, 200));
    panel.setOutlineThickness(2);
    panel.setOutlineColor(sf::Color(80, 90, 110));
    window->draw(panel);
    
    // Score text
    sf::Text scoreText("SCORE", font, 20);
    scoreText.setFillColor(sf::Color(200, 200, 255));
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 20);
    window->draw(scoreText);
    
    sf::Text scoreValue(std::to_string(score), font, 36);
    scoreValue.setFillColor(sf::Color(255, 255, 100));
    scoreValue.setStyle(sf::Text::Bold);
    scoreValue.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 50);
    window->draw(scoreValue);
    
    // Level text
    sf::Text levelText("LEVEL " + std::to_string(level), font, 18);
    levelText.setFillColor(textColor);
    levelText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 100);
    window->draw(levelText);
    
    // Length text
    sf::Text lengthText("LENGTH " + std::to_string(snake->getLength()), font, 18);
    lengthText.setFillColor(textColor);
    lengthText.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 130);
    window->draw(lengthText);
    
    // Controls
    sf::Text controlsTitle("CONTROLS", font, 16);
    controlsTitle.setFillColor(sf::Color(200, 200, 255));
    controlsTitle.setStyle(sf::Text::Bold);
    controlsTitle.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 220);
    window->draw(controlsTitle);
    
    std::vector<std::string> controls = {
        "WASD - Move",
        "P - Pause",
        "ESC - Menu"
    };
    
    for (size_t i = 0; i < controls.size(); i++) {
        sf::Text control(controls[i], font, 14);
        control.setFillColor(sf::Color(180, 180, 180));
        control.setPosition(panel.getPosition().x + 20, panel.getPosition().y + 250 + i * 20);
        window->draw(control);
    }
}

void Game::renderPauseMenu() {
    // Overlay
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window->draw(overlay);
    
    // Pause panel
    sf::RectangleShape panel(sf::Vector2f(400, 300));
    panel.setPosition((window->getSize().x - 400) / 2, (window->getSize().y - 300) / 2);
    panel.setFillColor(sf::Color(40, 45, 60, 240));
    panel.setOutlineThickness(3);
    panel.setOutlineColor(sf::Color(100, 120, 150));
    window->draw(panel);
    
    // Pause title
    sf::Text title("PAUSED", font, 40);
    title.setFillColor(sf::Color(255, 255, 100));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(panel.getPosition().x + (400 - titleBounds.width) / 2, panel.getPosition().y + 40);
    window->draw(title);
    
    // Options
    std::vector<std::string> options = {
        "SPACE - Resume",
        "R - Restart",
        "ESC - Main Menu"
    };
    
    for (size_t i = 0; i < options.size(); i++) {
        sf::Text option(options[i], font, 20);
        option.setFillColor(textColor);
        sf::FloatRect optionBounds = option.getLocalBounds();
        option.setPosition(panel.getPosition().x + (400 - optionBounds.width) / 2, 
                          panel.getPosition().y + 120 + i * 40);
        window->draw(option);
    }
}

void Game::renderGameOver() {
    // Overlay
    sf::RectangleShape overlay(sf::Vector2f(window->getSize().x, window->getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window->draw(overlay);
    
    // Game over panel
    sf::RectangleShape panel(sf::Vector2f(500, 400));
    panel.setPosition((window->getSize().x - 500) / 2, (window->getSize().y - 400) / 2);
    panel.setFillColor(sf::Color(60, 30, 30, 240));
    panel.setOutlineThickness(3);
    panel.setOutlineColor(sf::Color(150, 80, 80));
    window->draw(panel);
    
    // Game over title
    sf::Text title("GAME OVER", font, 40);
    title.setFillColor(sf::Color(255, 100, 100));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(panel.getPosition().x + (500 - titleBounds.width) / 2, panel.getPosition().y + 40);
    window->draw(title);
    
    // Final stats
    sf::Text finalScore("Final Score: " + std::to_string(score), font, 24);
    finalScore.setFillColor(sf::Color(255, 255, 100));
    sf::FloatRect scoreBounds = finalScore.getLocalBounds();
    finalScore.setPosition(panel.getPosition().x + (500 - scoreBounds.width) / 2, panel.getPosition().y + 120);
    window->draw(finalScore);
    
    sf::Text finalLevel("Level Reached: " + std::to_string(level), font, 20);
    finalLevel.setFillColor(textColor);
    sf::FloatRect levelBounds = finalLevel.getLocalBounds();
    finalLevel.setPosition(panel.getPosition().x + (500 - levelBounds.width) / 2, panel.getPosition().y + 160);
    window->draw(finalLevel);
    
    sf::Text finalLength("Snake Length: " + std::to_string(snake->getLength()), font, 20);
    finalLength.setFillColor(textColor);
    sf::FloatRect lengthBounds = finalLength.getLocalBounds();
    finalLength.setPosition(panel.getPosition().x + (500 - lengthBounds.width) / 2, panel.getPosition().y + 190);
    window->draw(finalLength);
    
    // Options
    std::vector<std::string> options = {
        "SPACE - Play Again",
        "H - High Scores",
        "ESC - Main Menu"
    };
    
    for (size_t i = 0; i < options.size(); i++) {
        sf::Text option(options[i], font, 18);
        option.setFillColor(textColor);
        sf::FloatRect optionBounds = option.getLocalBounds();
        option.setPosition(panel.getPosition().x + (500 - optionBounds.width) / 2, 
                          panel.getPosition().y + 270 + i * 30);
        window->draw(option);
    }
}

void Game::renderInstructions() {
    // Instructions panel
    sf::RectangleShape panel(sf::Vector2f(600, 500));
    panel.setPosition((window->getSize().x - 600) / 2, (window->getSize().y - 500) / 2);
    panel.setFillColor(sf::Color(40, 45, 60, 240));
    panel.setOutlineThickness(3);
    panel.setOutlineColor(sf::Color(100, 120, 150));
    window->draw(panel);
    
    // Title
    sf::Text title("HOW TO PLAY", font, 30);
    title.setFillColor(sf::Color(100, 255, 100));
    title.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(panel.getPosition().x + (600 - titleBounds.width) / 2, panel.getPosition().y + 30);
    window->draw(title);
    
    // Instructions text
    std::vector<std::string> instructions = {
        "• Use WASD keys to move the snake",
        "• Eat food (*) to grow and increase score",
        "• Avoid walls (#) and your own body",
        "• Game speed increases with each level",
        "• Multiple food items spawn simultaneously",
        "",
        "CONTROLS:",
        "• W - Move Up    • S - Move Down",
        "• A - Move Left  • D - Move Right",
        "• P - Pause Game",
        "• ESC - Return to Menu",
        "",
        "SCORING:",
        "• Each food item = 10 points",
        "• Level up every 100 points"
    };
    
    for (size_t i = 0; i < instructions.size(); i++) {
        sf::Text instruction(instructions[i], font, 16);
        instruction.setFillColor(instructions[i].empty() ? sf::Color::Transparent : textColor);
        if (instructions[i].find("CONTROLS:") != std::string::npos || 
            instructions[i].find("SCORING:") != std::string::npos) {
            instruction.setStyle(sf::Text::Bold);
            instruction.setFillColor(sf::Color(200, 200, 255));
        }
        instruction.setPosition(panel.getPosition().x + 50, panel.getPosition().y + 80 + i * 25);
        window->draw(instruction);
    }
    
    // Back option
    sf::Text back("Press ESC to return to menu", font, 18);
    back.setFillColor(sf::Color(255, 255, 100));
    sf::FloatRect backBounds = back.getLocalBounds();
    back.setPosition(panel.getPosition().x + (600 - backBounds.width) / 2, panel.getPosition().y + 450);
    window->draw(back);
}

void Game::handleInput() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            switch (gameState) {
                case MENU:
                    if (event.key.code == sf::Keyboard::Space) {
                        gameState = PLAYING;
                        // Reset game
                        delete snake;
                        snake = new Snake(Position(5, 5));
                        score = 0;
                        level = 1;
                        gameSpeed = 200;
                        food->clearFood();
                        graph->generateWalls(level);
                    } else if (event.key.code == sf::Keyboard::I) {
                        gameState = INSTRUCTIONS;
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        window->close();
                    }
                    break;
                    
                case PLAYING:
                    if (event.key.code == sf::Keyboard::W) {
                        snake->changeDirection(UP);
                    } else if (event.key.code == sf::Keyboard::S) {
                        snake->changeDirection(DOWN);
                    } else if (event.key.code == sf::Keyboard::A) {
                        snake->changeDirection(LEFT);
                    } else if (event.key.code == sf::Keyboard::D) {
                        snake->changeDirection(RIGHT);
                    } else if (event.key.code == sf::Keyboard::P) {
                        gameState = PAUSED;
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        gameState = MENU;
                    }
                    break;
                    
                case PAUSED:
                    if (event.key.code == sf::Keyboard::Space || event.key.code == sf::Keyboard::P) {
                        gameState = PLAYING;
                    } else if (event.key.code == sf::Keyboard::R) {
                        // Restart game
                        delete snake;
                        snake = new Snake(Position(5, 5));
                        score = 0;
                        level = 1;
                        gameSpeed = 200;
                        food->clearFood();
                        graph->generateWalls(level);
                        gameState = PLAYING;
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        gameState = MENU;
                    }
                    break;
                    
                case GAME_OVER:
                    if (event.key.code == sf::Keyboard::Space) {
                        // Play again
                        delete snake;
                        snake = new Snake(Position(5, 5));
                        score = 0;
                        level = 1;
                        gameSpeed = 200;
                        food->clearFood();
                        graph->generateWalls(level);
                        gameState = PLAYING;
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        gameState = MENU;
                    }
                    break;
                    
                case INSTRUCTIONS:
                    if (event.key.code == sf::Keyboard::Escape) {
                        gameState = MENU;
                    }
                    break;
            }
        }
    }
}

void Game::checkCollisions() {
    Position headPos = snake->getHeadPosition();
    
    // Wall collision
    if (graph->isWall(headPos)) {
        gameState = GAME_OVER;
        scoreManager->addScore(score, level);
        scoreManager->saveScoresToFile("scores.txt");
        return;
    }
    
    // Self collision
    if (snake->checkSelfCollision()) {
        gameState = GAME_OVER;
        scoreManager->addScore(score, level);
        scoreManager->saveScoresToFile("scores.txt");
        return;
    }
    
    // Food collision
    if (food->checkCollision(headPos)) {
        snake->grow();
        food->removeFood(headPos);
        score += 10;
        
        // Level up every 100 points
        if (score % 100 == 0) {
            nextLevel();
        }
    }
}

void Game::nextLevel() {
    level++;
    gameSpeed = std::max(50, gameSpeed - 20);
    graph->generateWalls(level);
    food->clearFood();
}

sf::Vector2f Game::getCellPosition(int x, int y) {
    return sf::Vector2f(BOARD_OFFSET_X + x * CELL_SIZE, BOARD_OFFSET_Y + y * CELL_SIZE);
}

void Game::drawGlowEffect(sf::Vector2f position, sf::Color color) {
    // Simple glow effect using multiple circles with decreasing alpha
    for (int i = 3; i >= 1; i--) {
        sf::CircleShape glow(CELL_SIZE / 2 + i * 2);
        glow.setPosition(position.x - i * 2, position.y - i * 2);
        sf::Color glowColor = color;
        glowColor.a = 20 / i;
        glow.setFillColor(glowColor);
        window->draw(glow);
    }
}