#include "Food.h"
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
Food::Food(int maxFoodCount) : maxFood(maxFoodCount), rng(std::random_device{}()) {}

void Food::spawn(int x, int y) {
    if (foodPositions.size() < maxFood) {
        foodPositions.push_back(sf::Vector2i(x, y));
    }
}

bool Food::loadTexture(const std::string& path) {
    if (!foodTexture.loadFromFile(path)) {
        std::cout << "Failed to load food texture: " << path << std::endl;
        return false;
    }
    foodSprite.setTexture(foodTexture);
    return true;
}

void Food::spawnRandom(int gridWidth, int gridHeight, 
                      const std::vector<sf::Vector2i>& occupiedPositions) {
    while (foodPositions.size() < maxFood) {
        int x = rng() % gridWidth;
        int y = rng() % gridHeight;

        // Check is position not occupied
        bool occupied = false;
        for (const auto& pos : occupiedPositions) {
            if (pos.x == x && pos.y == y) {
                occupied = true;
                break;
            }
        }
        
        // Check food exist
        for (const auto& food : foodPositions) {
            if (food.x == x && food.y == y) {
                occupied = true;
                break;
            }
        }
        
        if (!occupied) {
            foodPositions.push_back(sf::Vector2i(x, y));
        }
    }
}

bool Food::checkCollision(int x, int y) {
    auto it = std::find_if(foodPositions.begin(), foodPositions.end(),
                          [x, y](const sf::Vector2i& food) {
                              return food.x == x && food.y == y;
                          });
    
    if (it != foodPositions.end()) {
        foodPositions.erase(it);
        return true;
    }
    return false;
}

/*void Food::render(sf::RenderWindow& window, int cellSize) const {
    sf::CircleShape foodShape(cellSize / 2 - 2);
    foodShape.setFillColor(sf::Color::Red);
    
    for (const auto& food : foodPositions) {
        foodShape.setPosition(food.x * cellSize + 2, food.y * cellSize + 2);
        window.draw(foodShape);
    }
}*/

void Food::render(sf::RenderWindow& window, int cellSize)  {
    if (foodTexture.getSize().x == 0 || foodTexture.getSize().y == 0) {
    std::cerr << "Error: Food texture not loaded or empty!" << std::endl;
    return;
}

    for (const auto& food : foodPositions) {
        foodSprite.setPosition(food.x * cellSize, food.y * cellSize);
        foodSprite.setScale(
            static_cast<float>(cellSize) / foodTexture.getSize().x,
            static_cast<float>(cellSize) / foodTexture.getSize().y
        );
        window.draw(foodSprite);
    }
}

void Food::clear() {
    foodPositions.clear();
}