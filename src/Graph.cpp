#include "Graph.h"
#include <random>
#include <algorithm>

Graph::Graph(int w, int h) : width(w), height(h) {
    // Initialize graph with all possible connections
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            int current = getIndex(x, y);
            adjacencyList[current] = std::unordered_set<int>();
            
            // Add connections to adjacent cells
            if (x > 0) adjacencyList[current].insert(getIndex(x-1, y));
            if (x < width-1) adjacencyList[current].insert(getIndex(x+1, y));
            if (y > 0) adjacencyList[current].insert(getIndex(x, y-1));
            if (y < height-1) adjacencyList[current].insert(getIndex(x, y+1));
        }
    }
}

int Graph::getIndex(int x, int y) const {
    return y * width + x;
}

std::pair<int, int> Graph::getCoordinates(int index) const {
    return {index % width, index / width};
}

void Graph::addWall(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    
    int wallIndex = getIndex(x, y);
    
    // Remove all connections to this wall
    for (auto& pair : adjacencyList) {
        pair.second.erase(wallIndex);
    }
    
    // Clear the wall's connections
    adjacencyList[wallIndex].clear();
}

void Graph::removeWall(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    
    int current = getIndex(x, y);
    adjacencyList[current].clear();
    
    // Restore connections
    if (x > 0) {
        int left = getIndex(x-1, y);
        adjacencyList[current].insert(left);
        adjacencyList[left].insert(current);
    }
    if (x < width-1) {
        int right = getIndex(x+1, y);
        adjacencyList[current].insert(right);
        adjacencyList[right].insert(current);
    }
    if (y > 0) {
        int up = getIndex(x, y-1);
        adjacencyList[current].insert(up);
        adjacencyList[up].insert(current);
    }
    if (y < height-1) {
        int down = getIndex(x, y+1);
        adjacencyList[current].insert(down);
        adjacencyList[down].insert(current);
    }
}

bool Graph::isValidMove(int fromX, int fromY, int toX, int toY) const {
    if (toX < 0 || toX >= width || toY < 0 || toY >= height) return false;
    
    int fromIndex = getIndex(fromX, fromY);
    int toIndex = getIndex(toX, toY);
    
    auto it = adjacencyList.find(fromIndex);
    if (it != adjacencyList.end()) {
        return it->second.count(toIndex) > 0;
    }
    return false;
}

bool Graph::isWall(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return true;
    
    int index = getIndex(x, y);
    auto it = adjacencyList.find(index);
    return it != adjacencyList.end() && it->second.empty();
}

std::vector<std::pair<int, int>> Graph::getValidNeighbors(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors;
    int currentIndex = getIndex(x, y);
    
    auto it = adjacencyList.find(currentIndex);
    if (it != adjacencyList.end()) {
        for (int neighborIndex : it->second) {
            neighbors.push_back(getCoordinates(neighborIndex));
        }
    }
    
    return neighbors;
}

void Graph::clearWalls() {
    // Restore all connections
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            removeWall(x, y);
        }
    }
}

void Graph::generateWallLevel(int level) {
    clearWalls();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Generate walls based on level
    int wallCount = std::min(level * 3, (width * height) / 4);
    
    for (int i = 0; i < wallCount; i++) {
        int x = gen() % width;
        int y = gen() % height;
        
        // Don't place walls at starting position or too close to it
        if ((x < 3 && y < 3) || (x == width/2 && y == height/2)) {
            continue;
        }
        
        addWall(x, y);
    }
}