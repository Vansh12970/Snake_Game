#include "Snake.h"
#include <iostream>

Snake::Snake(Position startPos) : direction(RIGHT), length(1) {
    head = new SnakeNode(startPos);
    tail = head;
}

Snake::~Snake() {
    while (head) {
        SnakeNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void Snake::move() {
    Position newHeadPos = head->pos;
    
    switch (direction) {
        case UP: newHeadPos.y--; break;
        case DOWN: newHeadPos.y++; break;
        case LEFT: newHeadPos.x--; break;
        case RIGHT: newHeadPos.x++; break;
    }
    
    // Add new head
    SnakeNode* newHead = new SnakeNode(newHeadPos);
    newHead->next = head;
    head = newHead;
    
    // Remove tail if not growing
    if (static_cast<size_t>(length) == getBodyPositions().size()) {
        SnakeNode* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }
}

void Snake::grow() {
    length++;
}

void Snake::changeDirection(Direction newDir) {
    // Prevent reverse direction
    if ((direction == UP && newDir == DOWN) ||
        (direction == DOWN && newDir == UP) ||
        (direction == LEFT && newDir == RIGHT) ||
        (direction == RIGHT && newDir == LEFT)) {
        return;
    }
    direction = newDir;
}

bool Snake::checkSelfCollision() {
    Position headPos = head->pos;
    SnakeNode* current = head->next;
    
    while (current) {
        if (current->pos == headPos) {
            return true;
        }
        current = current->next;
    }
    return false;
}

Position Snake::getHeadPosition() {
    return head->pos;
}

std::vector<Position> Snake::getBodyPositions() {
    std::vector<Position> positions;
    SnakeNode* current = head;
    
    while (current) {
        positions.push_back(current->pos);
        current = current->next;
    }
    return positions;
}