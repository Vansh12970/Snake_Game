#include "Graph.h"
#include <random>
#include <ctime>
#include <algorithm>

Graph::Graph(int w, int h) : width(w), height(h) {
    walls.resize(height, std::vector<int>(width, 0));
    
    // Set border walls
    for (int i = 0; i < width; i++) {
        walls[0][i] = 1;
        walls[height - 1][i] = 1;
    }
    for (int i = 0; i < height; i++) {
        walls[i][0] = 1;
        walls[i][width - 1] = 1;
    }
}

void Graph::generateWalls(int level) {
    clearWalls();
    
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> xDist(2, width - 3);
    std::uniform_int_distribution<int> yDist(2, height - 3);
    
    int wallCount = std::min(level * 3, (width * height) / 8);
    
    for (int i = 0; i < wallCount; i++) {
        int x = xDist(rng);
        int y = yDist(rng);
        
        // Don't place walls too close to starting position
        if (x < 5 && y < 5) continue;
        
        walls[y][x] = 1;
    }
}

void Graph::clearWalls() {
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            walls[i][j] = 0;
        }
    }
}

bool Graph::isValidMove(Position pos) {
    return pos.x >= 0 && pos.x < width && 
           pos.y >= 0 && pos.y < height && 
           walls[pos.y][pos.x] == 0;
}

bool Graph::isWall(Position pos) {
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
        return true;
    }
    return walls[pos.y][pos.x] == 1;
}

std::vector<Position> Graph::findPath(Position start, Position end, const std::vector<Position>& obstacles) {
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::vector<std::vector<Position>> parent(height, std::vector<Position>(width, Position(-1, -1)));
    std::queue<Position> q;
    
    // Mark obstacles as visited
    for (const auto& obstacle : obstacles) {
        if (obstacle.x >= 0 && obstacle.x < width && obstacle.y >= 0 && obstacle.y < height) {
            visited[obstacle.y][obstacle.x] = true;
        }
    }
    
    q.push(start);
    visited[start.y][start.x] = true;
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    while (!q.empty()) {
        Position current = q.front();
        q.pop();
        
        if (current == end) {
            // Reconstruct path
            std::vector<Position> path;
            Position p = end;
            while (!(p.x == -1 && p.y == -1)) {
                path.push_back(p);
                p = parent[p.y][p.x];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        for (int i = 0; i < 4; i++) {
            Position next(current.x + dx[i], current.y + dy[i]);
            
            if (isValidMove(next) && !visited[next.y][next.x]) {
                visited[next.y][next.x] = true;
                parent[next.y][next.x] = current;
                q.push(next);
            }
        }
    }
    
    return std::vector<Position>(); // No path found
}