#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

struct Position {
    int x, y;
    Position(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

struct SnakeNode {
    Position pos;
    SnakeNode* next;
    SnakeNode(Position p) : pos(p), next(nullptr) {}
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class Snake {
private:
    SnakeNode* head;
    SnakeNode* tail;
    Direction direction;
    int length;

public:
    Snake(Position startPos);
    ~Snake();
    
    void move();
    void grow();
    void changeDirection(Direction newDir);
    bool checkSelfCollision();
    Position getHeadPosition();
    std::vector<Position> getBodyPositions();
    int getLength() const { return length; }
    Direction getDirection() const { return direction; }
};

#endif