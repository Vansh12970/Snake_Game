#ifndef FOOD_H
#define FOOD_H

#include "Snake.h"
#include <vector>
#include <random>

class Food {
private:
    std::vector<Position> foodPositions;
    std::mt19937 rng;
    int boardWidth, boardHeight;

public:
    Food(int width, int height);
    void spawnFood(const std::vector<Position>& snakeBody, const std::vector<std::vector<int>>& walls);
    bool checkCollision(Position pos);
    void removeFood(Position pos);
    std::vector<Position> getFoodPositions() const { return foodPositions; }
    void clearFood() { foodPositions.clear(); }
};

#endif