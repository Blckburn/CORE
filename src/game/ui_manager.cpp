#include "ui_manager.h"
#include "wave_manager.h"
#include "graphics/shader.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

UIManager::UIManager()
    : shader_(nullptr)
    , initialized_(false) {
}

UIManager::~UIManager() {
    Shutdown();
}

bool UIManager::Initialize(Shader* shader) {
    if (!shader) {
        std::cerr << "UIManager: Shader is null!" << std::endl;
        return false;
    }
    
    shader_ = shader;
    initialized_ = true;
    
    std::cout << "UI Manager initialized successfully!" << std::endl;
    return true;
}

void UIManager::Render(WaveManager* wave_manager, int window_width, int window_height) {
    if (!initialized_ || !wave_manager || !shader_) return;
    
    // Сохраняем текущее состояние
    GLboolean depth_test_enabled = glIsEnabled(GL_DEPTH_TEST);
    
    // Отключаем depth test для UI
    glDisable(GL_DEPTH_TEST);
    
    // Используем наш shader
    shader_->Use();
    
    // Создаём ортографическую проекцию для 2D UI
    glm::mat4 projection = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    shader_->SetUniform("projection", projection);
    shader_->SetUniform("view", view);
    
    // Получаем данные от WaveManager
    int current_wave = wave_manager->GetCurrentWave();
    int score = wave_manager->GetTotalScore();
    float time_till_wave = wave_manager->GetTimeTillNextWave();
    bool is_wave_active = wave_manager->IsWaveActive();
    
    // Рисуем UI элементы
    float ui_y = 20.0f;
    float ui_spacing = 50.0f;
    glm::vec3 cyan_color(0.0f, 1.0f, 1.0f);
    glm::vec3 green_color(0.0f, 1.0f, 0.0f);
    glm::vec3 red_color(1.0f, 0.0f, 0.0f);
    glm::vec3 yellow_color(1.0f, 1.0f, 0.0f);
    
    // Wave number
    RenderNumber(current_wave, 20.0f, ui_y, 2.0f, cyan_color);
    ui_y += ui_spacing;
    
    // Score
    RenderNumber(score, 20.0f, ui_y, 2.0f, green_color);
    ui_y += ui_spacing;
    
    // Time or enemies remaining
    if (!is_wave_active && time_till_wave > 0.0f) {
        int seconds = static_cast<int>(time_till_wave);
        RenderNumber(seconds, 20.0f, ui_y, 2.0f, yellow_color);
    } else {
        int enemies = wave_manager->GetEnemiesRemaining();
        RenderNumber(enemies, 20.0f, ui_y, 2.0f, red_color);
    }
    
    // Health bar (простой прогресс бар)
    float health_percent = 1.0f; // Пока 100%, позже добавим реальное здоровье
    RenderBar(window_width - 220.0f, 20.0f, 200.0f, 20.0f, health_percent, cyan_color);
    
    // Восстанавливаем состояние
    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
}

void UIManager::RenderBar(float x, float y, float width, float height, float fill_percent, const glm::vec3& color) {
    glm::mat4 model = glm::mat4(1.0f);
    shader_->SetUniform("model", model);
    
    // Рисуем рамку
    float outline_vertices[] = {
        x, y,
        x + width, y,
        x + width, y + height,
        x, y + height
    };
    
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(outline_vertices), outline_vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    shader_->SetUniform("color", color * 0.3f);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    
    // Рисуем заполнение
    float fill_width = width * fill_percent;
    float fill_vertices[] = {
        x + 2, y + 2,
        x + fill_width - 2, y + 2,
        x + fill_width - 2, y + height - 2,
        x + 2, y + 2,
        x + fill_width - 2, y + height - 2,
        x + 2, y + height - 2
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fill_vertices), fill_vertices, GL_STATIC_DRAW);
    
    shader_->SetUniform("color", color);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

void UIManager::RenderNumber(int number, float x, float y, float scale, const glm::vec3& color) {
    if (number == 0) {
        RenderDigit(0, x, y, scale, color);
        return;
    }
    
    // Получаем цифры числа
    std::string num_str = std::to_string(number);
    float offset_x = 0;
    
    for (char c : num_str) {
        int digit = c - '0';
        RenderDigit(digit, x + offset_x, y, scale, color);
        offset_x += 15.0f * scale; // Ширина цифры
    }
}

void UIManager::RenderDigit(int digit, float x, float y, float scale, const glm::vec3& color) {
    glm::mat4 model = glm::mat4(1.0f);
    shader_->SetUniform("model", model);
    shader_->SetUniform("color", color);
    
    glLineWidth(2.0f * scale);
    
    // Простые сегменты для цифр (7-сегментный дисплей стиль)
    float w = 10.0f * scale;
    float h = 20.0f * scale;
    
    // Определяем какие сегменты рисовать для каждой цифры
    bool segments[7] = {false};
    
    switch(digit) {
        case 0: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=0; segments[4]=1; segments[5]=1; segments[6]=1; break;
        case 1: segments[0]=0; segments[1]=0; segments[2]=1; segments[3]=0; segments[4]=0; segments[5]=1; segments[6]=0; break;
        case 2: segments[0]=1; segments[1]=0; segments[2]=1; segments[3]=1; segments[4]=1; segments[5]=0; segments[6]=1; break;
        case 3: segments[0]=1; segments[1]=0; segments[2]=1; segments[3]=1; segments[4]=0; segments[5]=1; segments[6]=1; break;
        case 4: segments[0]=0; segments[1]=1; segments[2]=1; segments[3]=1; segments[4]=0; segments[5]=1; segments[6]=0; break;
        case 5: segments[0]=1; segments[1]=1; segments[2]=0; segments[3]=1; segments[4]=0; segments[5]=1; segments[6]=1; break;
        case 6: segments[0]=1; segments[1]=1; segments[2]=0; segments[3]=1; segments[4]=1; segments[5]=1; segments[6]=1; break;
        case 7: segments[0]=1; segments[1]=0; segments[2]=1; segments[3]=0; segments[4]=0; segments[5]=1; segments[6]=0; break;
        case 8: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=1; segments[4]=1; segments[5]=1; segments[6]=1; break;
        case 9: segments[0]=1; segments[1]=1; segments[2]=1; segments[3]=1; segments[4]=0; segments[5]=1; segments[6]=1; break;
    }
    
    // Собираем все линии в один массив
    std::vector<float> vertices;
    
    // Segment 0 (top)
    if (segments[0]) { 
        vertices.push_back(x); vertices.push_back(y);
        vertices.push_back(x + w); vertices.push_back(y);
    }
    
    // Segment 1 (top-left)
    if (segments[1]) { 
        vertices.push_back(x); vertices.push_back(y);
        vertices.push_back(x); vertices.push_back(y + h/2);
    }
    
    // Segment 2 (top-right)
    if (segments[2]) { 
        vertices.push_back(x + w); vertices.push_back(y);
        vertices.push_back(x + w); vertices.push_back(y + h/2);
    }
    
    // Segment 3 (middle)
    if (segments[3]) { 
        vertices.push_back(x); vertices.push_back(y + h/2);
        vertices.push_back(x + w); vertices.push_back(y + h/2);
    }
    
    // Segment 4 (bottom-left)
    if (segments[4]) { 
        vertices.push_back(x); vertices.push_back(y + h/2);
        vertices.push_back(x); vertices.push_back(y + h);
    }
    
    // Segment 5 (bottom-right)
    if (segments[5]) { 
        vertices.push_back(x + w); vertices.push_back(y + h/2);
        vertices.push_back(x + w); vertices.push_back(y + h);
    }
    
    // Segment 6 (bottom)
    if (segments[6]) { 
        vertices.push_back(x); vertices.push_back(y + h);
        vertices.push_back(x + w); vertices.push_back(y + h);
    }
    
    if (vertices.empty()) return;
    
    // Рисуем все линии
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_LINES, 0, vertices.size() / 2);
    
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    
    glLineWidth(1.0f);
}

void UIManager::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    // Заглушка для будущей реализации текста
    // Пока используем только цифры
}

void UIManager::Shutdown() {
    initialized_ = false;
}

