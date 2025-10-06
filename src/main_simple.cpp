#include <iostream>
#include <windows.h>

int main() {
    std::cout << "=== CORE - Minimalist 3D Tower Defense ===" << std::endl;
    std::cout << "Starting engine initialization..." << std::endl;
    
    // Simple console-based game loop for now
    std::cout << "Engine initialized successfully!" << std::endl;
    std::cout << "Game loop starting..." << std::endl;
    
    bool running = true;
    int wave = 1;
    int core_hp = 100;
    int energy = 50;
    
    std::cout << "\n=== GAME START ===" << std::endl;
    std::cout << "Wave: " << wave << std::endl;
    std::cout << "Core HP: " << core_hp << std::endl;
    std::cout << "Energy: " << energy << std::endl;
    std::cout << "\nCommands:" << std::endl;
    std::cout << "  'p' - Place turret (cost: 10 energy)" << std::endl;
    std::cout << "  'n' - Next wave" << std::endl;
    std::cout << "  'q' - Quit" << std::endl;
    
    while (running && core_hp > 0) {
        std::cout << "\n> ";
        char input;
        std::cin >> input;
        
        switch (input) {
            case 'p':
                if (energy >= 10) {
                    energy -= 10;
                    std::cout << "Turret placed! Energy: " << energy << std::endl;
                } else {
                    std::cout << "Not enough energy! Need 10, have " << energy << std::endl;
                }
                break;
                
            case 'n':
                wave++;
                core_hp -= 5; // Simulate enemy damage
                energy += 20; // Gain energy from wave
                std::cout << "Wave " << wave << " started!" << std::endl;
                std::cout << "Core HP: " << core_hp << " (-5 from enemy)" << std::endl;
                std::cout << "Energy: " << energy << " (+20 from wave)" << std::endl;
                break;
                
            case 'q':
                running = false;
                std::cout << "Quitting game..." << std::endl;
                break;
                
            default:
                std::cout << "Unknown command. Use 'p', 'n', or 'q'" << std::endl;
                break;
        }
        
        if (core_hp <= 0) {
            std::cout << "\n=== GAME OVER ===" << std::endl;
            std::cout << "Your core has been destroyed!" << std::endl;
            std::cout << "Final wave reached: " << wave << std::endl;
        }
    }
    
    std::cout << "\n=== CORE Engine Shutdown ===" << std::endl;
    std::cout << "Thank you for playing CORE!" << std::endl;
    
    return 0;
}

