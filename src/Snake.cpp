#include "Snake.h"

Snake::Snake(int startX, int startY) : direction(RIGHT), nextDirection(RIGHT), growing(false) {
    body.push_back(SnakeSegment(startX, startY));
    body.push_back(SnakeSegment(startX - 1, startY));
    body.push_back(SnakeSegment(startX - 2, startY));
}

void Snake::move() {
    direction = nextDirection;
    
    SnakeSegment head = body.front();
    SnakeSegment newHead = head;
    
    switch (direction) {
        case UP: newHead.y--; break;
        case DOWN: newHead.y++; break;
        case LEFT: newHead.x--; break;
        case RIGHT: newHead.x++; break;
    }
    
    body.push_front(newHead);
    
    if (!growing) {
        body.pop_back();
    } else {
        growing = false;
    }
}

void Snake::grow() {
    growing = true;
}

void Snake::setDirection(Direction dir) {
    // Prevent immediate reversal
    if ((direction == UP && dir != DOWN) ||
        (direction == DOWN && dir != UP) ||
        (direction == LEFT && dir != RIGHT) ||
        (direction == RIGHT && dir != LEFT)) {
        nextDirection = dir;
    }
}

bool Snake::checkSelfCollision() const {
    const SnakeSegment& head = body.front();
    auto it = body.begin();
    ++it; // Skip head
    
    for (; it != body.end(); ++it) {
        if (head.x == it->x && head.y == it->y) {
            return true;
        }
    }
    return false;
}

bool Snake::checkCollision(int x, int y) const {
    for (const auto& segment : body) {
        if (segment.x == x && segment.y == y) {
            return true;
        }
    }
    return false;
}

void Snake::render(sf::RenderWindow& window, int cellSize) const {
    sf::RectangleShape segment(sf::Vector2f(cellSize - 2, cellSize - 2));
    
    bool isHead = true;
    for (const auto& bodySegment : body) {
        segment.setPosition(bodySegment.x * cellSize + 1, bodySegment.y * cellSize + 1);
        
        if (isHead) {
            segment.setFillColor(sf::Color::Green);
            isHead = false;
        } else {
            segment.setFillColor(sf::Color(100, 255, 100));
        }
        
        window.draw(segment);
    }
}

SnakeSegment Snake::getHead() const {
    return body.front();
}

int Snake::getLength() const {
    return body.size();
}

void Snake::reset(int startX, int startY) {
    body.clear();
    body.push_back(SnakeSegment(startX, startY));
    body.push_back(SnakeSegment(startX - 1, startY));
    body.push_back(SnakeSegment(startX - 2, startY));
    direction = RIGHT;
    nextDirection = RIGHT;
    growing = false;
}