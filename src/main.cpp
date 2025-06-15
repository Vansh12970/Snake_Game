#include <iostream>
#include "Game.h"

int main() {
    try {
        std::cout << "Starting Snake Game with Graphics...\n";
        
        Game game;
        game.run();
        
        std::cout << "Game ended. Thanks for playing!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}