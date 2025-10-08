// Main game engine - initializes and coordinates all game systems
#pragma once

#include <memory>
#include <string>

class Window;
class Renderer;
class InputManager;
class Game;

class Engine {
public:
    Engine();
    ~Engine();
    
    // Initialize all engine systems
    bool Initialize();
    
    // Main game loop
    void Run();
    
    // Shutdown all systems
    void Shutdown();
    
    // Get engine subsystems
    Window* GetWindow() const { return window_.get(); }
    Renderer* GetRenderer() const { return renderer_.get(); }
    InputManager* GetInput() const { return input_.get(); }
    Game* GetGame() const { return game_.get(); }
    
    // Engine state
    bool IsRunning() const { return is_running_; }
    void RequestShutdown() { is_running_ = false; }
    
private:
    bool is_running_;
    
    // Core systems
    std::unique_ptr<Window> window_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<InputManager> input_;
    std::unique_ptr<Game> game_;
    
    // Initialize individual systems
    bool InitializeWindow();
    bool InitializeRenderer();
    bool InitializeInput();
    bool InitializeGame();
};


