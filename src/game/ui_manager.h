#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader;
class WaveManager;

class UIManager {
public:
    UIManager();
    ~UIManager();

    bool Initialize(Shader* shader);
    void Render(WaveManager* wave_manager, int window_width, int window_height);
    void Shutdown();

private:
    Shader* shader_;
    bool initialized_;
    
    // Простой текстовый рендеринг через OpenGL примитивы
    void RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color);
    void RenderDigit(int digit, float x, float y, float scale, const glm::vec3& color);
    void RenderNumber(int number, float x, float y, float scale, const glm::vec3& color);
    void RenderBar(float x, float y, float width, float height, float fill_percent, const glm::vec3& color);
};

