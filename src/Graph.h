#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>

class Graph {
private:
    int width, height;
    std::unordered_map<int, std::unordered_set<int>> adjacencyList;
    
    int getIndex(int x, int y) const;
    std::pair<int, int> getCoordinates(int index) const;

public:
    Graph(int w, int h);
    void addWall(int x, int y);
    void removeWall(int x, int y);
    bool isValidMove(int fromX, int fromY, int toX, int toY) const;
    bool isWall(int x, int y) const;
    std::vector<std::pair<int, int>> getValidNeighbors(int x, int y) const;
    void clearWalls();
    void generateWallLevel(int level);
};

#endif