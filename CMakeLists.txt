cmake_minimum_required(VERSION 3.16)
project(SnakeGame)

set(CMAKE_CXX_STANDARD 17)

# Find SFML
find_package(sfml REQUIRED COMPONENTS system window graphics audio)

# Add executable
add_executable(SnakeGame
    src/main.cpp
    src/Game.cpp
    src/Snake.cpp
    src/Food.cpp
    src/ScoreManager.cpp
    src/Graph.cpp
)

# Link SFML
target_link_libraries(SnakeGame sfml-system sfml-window sfml-graphics sfml-audio)

# Include directories
target_include_directories(SnakeGame PRIVATE src)