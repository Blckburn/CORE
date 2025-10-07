#pragma once

#include <memory>
#include "camera.h"

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
    int GetViewportWidth() const;
    int GetViewportHeight() const;
    void SetWindowSize(int width, int height);
    
private:
    Window* window_;
    std::shared_ptr<Camera> camera_;
    
    float clear_color_[4];
};


