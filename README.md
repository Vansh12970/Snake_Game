# Snake Game with Data Structures

A modern Snake game implementation in C++ using SFML, featuring various data structures and algorithms.

## Features

### Data Structures Used:
- **Linked List**: Snake body representation for efficient insertion/deletion
- **Stack**: Score history for undo functionality
- **Queue**: Recent scores tracking
- **Graph**: Wall layout and movement validation
- **Array/Vector**: Food positions and high scores storage

### Game Features:
- Dynamic food spawning (multiple food items)
- Progressive wall levels using graph-based movement validation
- Score history with undo functionality
- High scores persistence
- Pause/Resume functionality
- Modern GUI with SFML

## Prerequisites

- C++17 compatible compiler
- SFML 2.5+ library
- CMake 3.16+

## Installation

### Windows (Visual Studio)
1. Install vcpkg
2. Install SFML: `vcpkg install sfml`
3. Build with CMake or Visual Studio

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libsfml-dev cmake build-essential