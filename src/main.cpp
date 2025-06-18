#include "Game.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "Starting Snake Game..." << std::endl;
    
    try {
        std::cout << "Creating Game object..." << std::endl;
        Game game;
        
        std::cout << "Calling game.run()..." << std::endl;
        game.run();
        
        std::cout << "Game finished normally." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cout << "Press any key to continue..." << std::endl;
        std::cin.get();
        return -1;
    }
    
    std::cout << "Game ended. Press any key to exit..." << std::endl;
    std::cin.get();
    return 0;
}