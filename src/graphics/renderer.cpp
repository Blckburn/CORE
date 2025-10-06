#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "core/window.h"
#include <iostream>

Renderer::Renderer() : window_(nullptr) {
    clear_color_[0] = 0.0f; // Black background
    clear_color_[1] = 0.0f;
    clear_color_[2] = 0.0f;
    clear_color_[3] = 1.0f;
}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize(Window* window) {
    std::cout << "Initializing renderer..." << std::endl;
    
    if (!window) {
        std::cerr << "Renderer: Window is null!" << std::endl;
        return false;
    }
    
    window_ = window;
    
    // Set OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set viewport
    glViewport(0, 0, window_->GetWidth(), window_->GetHeight());
    
    std::cout << "Renderer initialized successfully!" << std::endl;
    return true;
}

void Renderer::Shutdown() {
    std::cout << "Shutting down renderer..." << std::endl;
    window_ = nullptr;
    camera_.reset();
}

void Renderer::BeginFrame() {
    // Clear screen
    Clear();
}

void Renderer::EndFrame() {
    // Nothing to do here for now
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    clear_color_[0] = r;
    clear_color_[1] = g;
    clear_color_[2] = b;
    clear_color_[3] = a;
}

void Renderer::Clear() {
    glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetCamera(std::shared_ptr<Camera> camera) {
    camera_ = camera;
}
