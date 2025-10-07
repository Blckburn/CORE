#include <iostream>
#include <memory>
#include "core/engine.h"

int main() {
    std::cout << "Starting CORE - Minimalist 3D Tower Defense" << std::endl;
    
    try {
        // Create and initialize engine
        auto engine = std::make_unique<Engine>();
        
        if (!engine->Initialize()) {
            std::cerr << "Failed to initialize engine!" << std::endl;
            return -1;
        }
        
        std::cout << "Engine initialized successfully!" << std::endl;
        
        // Run the main game loop
        engine->Run();
        
        std::cout << "Engine shutdown complete!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}


