#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <conio.h> // For Windows - use termios.h for Linux/Mac
#include <windows.h> // For Windows console manipulation

Game::Game(int width, int height) : boardWidth(width), boardHeight(height), 
                                   score(0), level(1), gameOver(false), paused(false), gameSpeed(200) {
    snake = new Snake(Position(5, 5));
    food = new Food(width, height);
    graph = new Graph(width, height);
    scoreManager = new ScoreManager();
    
    // Load previous scores
    scoreManager->loadScoresFromFile("scores.txt");
}

Game::~Game() {
    delete snake;
    delete food;
    delete graph;
    delete scoreManager;
}

void Game::initialize() {
    graph->generateWalls(level);
    food->spawnFood(snake->getBodyPositions(), graph->getWalls());
    lastUpdate = std::chrono::steady_clock::now();
}

void Game::run() {
    initialize();
    showInstructions();
    
    while (!gameOver) {
        if (!paused) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
            
            if (elapsed.count() >= gameSpeed) {
                update();
                lastUpdate = now;
            }
        }
        
        handleInput();
        render();
        Sleep(50); // Small delay to prevent excessive CPU usage
    }
    
    gameOverScreen();
}

void Game::update() {
    snake->move();
    checkCollisions();
    food->spawnFood(snake->getBodyPositions(), graph->getWalls());
}

void Game::render() {
    system("cls"); // Clear screen (Windows) - use system("clear") for Linux/Mac
    
    // Game info
    std::cout << "Score: " << score << " | Level: " << level << " | Length: " << snake->getLength();
    if (paused) std::cout << " | PAUSED";
    std::cout << "\n";
    
    // Draw board
    for (int y = 0; y < boardHeight; y++) {
        for (int x = 0; x < boardWidth; x++) {
            Position pos(x, y);
            
            // Check walls
            if (graph->isWall(pos)) {
                std::cout << "#";
                continue;
            }
            
            // Check snake
            bool isSnake = false;
            auto snakeBody = snake->getBodyPositions();
            for (size_t i = 0; i < snakeBody.size(); i++) {
                if (snakeBody[i] == pos) {
                    if (i == 0) std::cout << "O"; // Head
                    else std::cout << "o"; // Body
                    isSnake = true;
                    break;
                }
            }
            if (isSnake) continue;
            
            // Check food
            if (food->checkCollision(pos)) {
                std::cout << "*";
                continue;
            }
            
            // Empty space
            std::cout << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "\nControls: WASD to move, P to pause, Q to quit, H for help\n";
}

void Game::handleInput() {
    if (_kbhit()) { // Check if key is pressed (Windows)
        char key = getKeyPress();
        
        switch (key) {
            case 'w': case 'W':
                snake->changeDirection(UP);
                break;
            case 's': case 'S':
                snake->changeDirection(DOWN);
                break;
            case 'a': case 'A':
                snake->changeDirection(LEFT);
                break;
            case 'd': case 'D':
                snake->changeDirection(RIGHT);
                break;
            case 'p': case 'P':
                paused = !paused;
                if (paused) pauseMenu();
                break;
            case 'q': case 'Q':
                gameOver = true;
                break;
            case 'h': case 'H':
                showInstructions();
                break;
        }
    }
}

void Game::checkCollisions() {
    Position headPos = snake->getHeadPosition();
    
    // Wall collision
    if (graph->isWall(headPos)) {
        gameOver = true;
        return;
    }
    
    // Self collision
    if (snake->checkSelfCollision()) {
        gameOver = true;
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
    gameSpeed = std::max(50, gameSpeed - 20); // Increase speed
    graph->generateWalls(level);
    food->clearFood();
    
    std::cout << "\nLevel Up! Level " << level << "\n";
    std::cout << "Press any key to continue...";
    _getch();
}

void Game::gameOverScreen() {
    system("cls");
    std::cout << "========================================\n";
    std::cout << "           GAME OVER!\n";
    std::cout << "========================================\n";
    std::cout << "Final Score: " << score << "\n";
    std::cout << "Level Reached: " << level << "\n";
    std::cout << "Snake Length: " << snake->getLength() << "\n";
    
    // Add score to manager
    scoreManager->addScore(score, level);
    scoreManager->saveScoresToFile("scores.txt");
    
    std::cout << "\nWhat would you like to see?\n";
    std::cout << "1. High Scores\n";
    std::cout << "2. Recent Scores\n";
    std::cout << "3. Score History\n";
    std::cout << "4. Exit\n";
    std::cout << "Choice: ";
    
    char choice = _getch();
    switch (choice) {
        case '1':
            scoreManager->displayHighScores();
            break;
        case '2':
            scoreManager->displayRecentScores();
            break;
        case '3':
            scoreManager->displayScoreHistory();
            break;
    }
    
    std::cout << "\nPress any key to exit...";
    _getch();
}

void Game::pauseMenu() {
    system("cls");
    std::cout << "========================================\n";
    std::cout << "              PAUSED\n";
    std::cout << "========================================\n";
    std::cout << "Current Score: " << score << "\n";
    std::cout << "Current Level: " << level << "\n";
    std::cout << "\n1. Resume Game\n";
    std::cout << "2. View High Scores\n";
    std::cout << "3. View Recent Scores\n";
    std::cout << "4. Instructions\n";
    std::cout << "5. Quit Game\n";
    std::cout << "\nChoice: ";
    
    char choice = _getch();
    switch (choice) {
        case '1':
            paused = false;
            break;
        case '2':
            scoreManager->displayHighScores();
            std::cout << "\nPress any key to continue...";
            _getch();
            break;
        case '3':
            scoreManager->displayRecentScores();
            std::cout << "\nPress any key to continue...";
            _getch();
            break;
        case '4':
            showInstructions();
            break;
        case '5':
            gameOver = true;
            paused = false;
            break;
    }
}

void Game::showInstructions() {
    system("cls");
    std::cout << "========================================\n";
    std::cout << "         SNAKE GAME INSTRUCTIONS\n";
    std::cout << "========================================\n";
    std::cout << "\nObjective:\n";
    std::cout << "- Eat food (*) to grow and increase score\n";
    std::cout << "- Avoid walls (#) and your own body\n";
    std::cout << "- Survive as long as possible!\n";
    std::cout << "\nControls:\n";
    std::cout << "- W: Move Up\n";
    std::cout << "- S: Move Down\n";
    std::cout << "- A: Move Left\n";
    std::cout << "- D: Move Right\n";
    std::cout << "- P: Pause/Resume\n";
    std::cout << "- Q: Quit Game\n";
    std::cout << "- H: Show Help\n";
    std::cout << "\nGame Features:\n";
    std::cout << "- Dynamic food spawning (multiple food items)\n";
    std::cout << "- Wall levels that increase with progression\n";
    std::cout << "- Score history tracking with stack/queue\n";
    std::cout << "- Speed increases with each level\n";
    std::cout << "\nPress any key to continue...";
    _getch();
}

char Game::getKeyPress() {
    return _getch(); // Windows - use different method for Linux/Mac
}