#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <list>

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x = 0, int y = 0) : x(x), y(y) {}
};

class Snake {
private:
    std::list<SnakeSegment> body;
    Direction direction;
    Direction nextDirection;
    bool growing;

public:
    Snake(int startX, int startY);
    void move();
    void grow();
    void setDirection(Direction dir);
    bool checkSelfCollision() const;
    bool checkCollision(int x, int y) const;
    void render(sf::RenderWindow& window, int cellSize) const;
    SnakeSegment getHead() const;
    int getLength() const;
    void reset(int startX, int startY);
    std::list<SnakeSegment> getBody() const { return body; }
};

#endif