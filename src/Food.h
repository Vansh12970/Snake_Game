#ifndef FOOD_H
#define FOOD_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class Food {
private:
    std::vector<sf::Vector2i> foodPositions;
    std::mt19937 rng;
    int maxFood;
    sf::Texture foodTexture; // New: texture for the food icon
    sf::Sprite foodSprite;   // New: sprite used for drawing
public:
    Food(int maxFoodCount = 3);
    void spawn(int x, int y);
    void spawnRandom(int gridWidth, int gridHeight, 
                    const std::vector<sf::Vector2i>& occupiedPositions);
    bool checkCollision(int x, int y);
    void render(sf::RenderWindow& window, int cellSize);
    void clear();
    bool loadTexture(const std::string& path);
    int getCount() const { return foodPositions.size(); }
    std::vector<sf::Vector2i> getPositions() const { return foodPositions; }
};

#endif