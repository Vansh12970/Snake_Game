#include <iostream>
#include "Game.h"

int main() {
    try {
        std::cout << "Starting Snake Game with Beautiful GUI...\n";
        
        Game game(30, 20); // Slightly larger board for better visual experience
        game.run();
        
        std::cout << "Game ended. Thanks for playing!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return -1;
    }
    
    return 0;
}