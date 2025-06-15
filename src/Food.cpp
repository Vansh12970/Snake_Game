#include "Food.h"
#include <algorithm>
#include <ctime>

Food::Food(int width, int height) : boardWidth(width), boardHeight(height) {
    rng.seed(static_cast<unsigned int>(std::time(nullptr)));
}

void Food::spawnFood(const std::vector<Position>& snakeBody, const std::vector<std::vector<int>>& walls) {
    // Maintain 2-4 food items on the board
    if (foodPositions.size() >= 4) return;
    
    std::uniform_int_distribution<int> xDist(1, boardWidth - 2);
    std::uniform_int_distribution<int> yDist(1, boardHeight - 2);
    
    int attempts = 0;
    while (foodPositions.size() < 3 && attempts < 100) {
        Position newFood(xDist(rng), yDist(rng));
        
        // Check if position is valid (not on snake, not on wall, not on existing food)
        bool valid = true;
        
        // Check snake collision
        for (const auto& bodyPos : snakeBody) {
            if (bodyPos == newFood) {
                valid = false;
                break;
            }
        }
        
        // Check wall collision
        if (valid && walls[newFood.y][newFood.x] == 1) {
            valid = false;
        }
        
        // Check existing food collision
        if (valid) {
            for (const auto& food : foodPositions) {
                if (food == newFood) {
                    valid = false;
                    break;
                }
            }
        }
        
        if (valid) {
            foodPositions.push_back(newFood);
        }
        attempts++;
    }
}

bool Food::checkCollision(Position pos) {
    for (const auto& food : foodPositions) {
        if (food == pos) {
            return true;
        }
    }
    return false;
}

void Food::removeFood(Position pos) {
    foodPositions.erase(
        std::remove(foodPositions.begin(), foodPositions.end(), pos),
        foodPositions.end()
    );
}