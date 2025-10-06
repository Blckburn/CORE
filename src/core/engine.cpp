#include "engine.h"
#include "window.h"
#include "graphics/renderer.h"
#include "input.h"
#include "game/game.h"
#include "time.h"
#include <iostream>

Engine::Engine() : is_running_(false) {
}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize() {
    std::cout << "Initializing CORE Engine..." << std::endl;
    
    // Initialize time system
    Time::Initialize();
    
    // Initialize systems in order
    if (!InitializeWindow()) {
        std::cerr << "Failed to initialize window!" << std::endl;
        return false;
    }
    
    if (!InitializeRenderer()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return false;
    }
    
    if (!InitializeInput()) {
        std::cerr << "Failed to initialize input!" << std::endl;
        return false;
    }
    
    if (!InitializeGame()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return false;
    }
    
    is_running_ = true;
    std::cout << "CORE Engine initialized successfully!" << std::endl;
    
    return true;
}

void Engine::Run() {
    if (!is_running_) {
        std::cerr << "Engine not initialized!" << std::endl;
        return;
    }
    
    std::cout << "Starting main game loop..." << std::endl;
    
    while (is_running_ && window_->ShouldClose()) {
        // Update time
        Time::Update();
        
        // Update input
        input_->Update();
        
        // Update game logic
        game_->Update();
        
        // Render frame
        renderer_->BeginFrame();
        game_->Render();
        renderer_->EndFrame();
        
        // Swap buffers and poll events
        window_->SwapBuffers();
        window_->PollEvents();
    }
    
    std::cout << "Main game loop ended." << std::endl;
}

void Engine::Shutdown() {
    std::cout << "Shutting down CORE Engine..." << std::endl;
    
    is_running_ = false;
    
    // Shutdown in reverse order
    game_.reset();
    input_.reset();
    renderer_.reset();
    window_.reset();
    
    std::cout << "CORE Engine shutdown complete!" << std::endl;
}

bool Engine::InitializeWindow() {
    window_ = std::make_unique<Window>();
    return window_->Initialize(1280, 720, "CORE");
}

bool Engine::InitializeRenderer() {
    renderer_ = std::make_unique<Renderer>();
    return renderer_->Initialize(window_.get());
}

bool Engine::InitializeInput() {
    input_ = std::make_unique<InputManager>(window_->GetGLFWWindow());
    return true;
}

bool Engine::InitializeGame() {
    game_ = std::make_unique<Game>();
    return game_->Initialize(renderer_.get(), input_.get());
}
