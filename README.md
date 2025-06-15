# Advanced Snake Game in C++

A feature-rich Snake game implementation using advanced data structures and algorithms.

## Features

- **Linked List**: Snake body management
- **Arrays**: Game board representation
- **Graph Theory**: Wall system and pathfinding
- **Stack**: Score history tracking
- **Queue**: Recent scores management
- **Dynamic Programming**: Food spawning algorithm

## Data Structures Used

1. **Linked List** - Snake body segments
2. **2D Arrays** - Game board and wall system
3. **Graph** - Movement validation and pathfinding
4. **Stack** - Score history (LIFO)
5. **Queue** - Recent scores (FIFO)
6. **Vector** - Dynamic food management

## Compilation

### Windows (MinGW)
```bash
g++ -std=c++11 -Wall -Wextra -O2 src/*.cpp -o SnakeGame
RUNNING THE GAME
./SnakeGame
Controls

W/A/S/D: Movement
P: Pause/Resume
Q: Quit
H: Help

Game Mechanics

Eat food (*) to grow and score points
Avoid walls (#) and your own body
Game speed increases with each level
Multiple food items spawn dynamically
Wall complexity increases with progression

Score System

10 points per food item
Level up every 100 points
Scores saved to file automatically
View high scores, recent scores, and complete history