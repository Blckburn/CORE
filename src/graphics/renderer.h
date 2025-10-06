#pragma once

#include <memory>

class Window;
class Camera;

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    bool Initialize(Window* window);
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void SetClearColor(float r, float g, float b, float a);
    void Clear();
    
    void SetCamera(std::shared_ptr<Camera> camera);
    
private:
    Window* window_;
    std::shared_ptr<Camera> camera_;
    
    float clear_color_[4];
};

