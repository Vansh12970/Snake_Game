#ifndef GRAPH_H
#define GRAPH_H

#include "Snake.h"
#include <vector>
#include <queue>

class Graph {
private:
    int width, height;
    std::vector<std::vector<int>> walls;
    
public:
    Graph(int w, int h);
    void generateWalls(int level);
    bool isValidMove(Position pos);
    bool isWall(Position pos);
    std::vector<std::vector<int>> getWalls() const { return walls; }
    std::vector<Position> findPath(Position start, Position end, const std::vector<Position>& obstacles);
    void clearWalls();
};

#endif