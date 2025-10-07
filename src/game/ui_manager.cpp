#include "ui_manager.h"
#include "wave_manager.h"
#include "graphics/shader.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <glad/glad.h>

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
    if (!initialized_ || !wave_manager) return;
    
    // Используем ортографическую проекцию для 2D UI
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, window_width, window_height, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Отключаем depth test для UI
    glDisable(GL_DEPTH_TEST);
    
    // Получаем данные от WaveManager
    int current_wave = wave_manager->GetCurrentWave();
    int score = wave_manager->GetTotalScore();
    float time_till_wave = wave_manager->GetTimeTillNextWave();
    bool is_wave_active = wave_manager->IsWaveActive();
    
    // Рисуем UI элементы
    float ui_y = 20.0f;
    float ui_spacing = 40.0f;
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
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void UIManager::RenderBar(float x, float y, float width, float height, float fill_percent, const glm::vec3& color) {
    // Рисуем рамку
    glColor3f(color.r * 0.3f, color.g * 0.3f, color.b * 0.3f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Рисуем заполнение
    float fill_width = width * fill_percent;
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(x + 2, y + 2);
    glVertex2f(x + fill_width - 2, y + 2);
    glVertex2f(x + fill_width - 2, y + height - 2);
    glVertex2f(x + 2, y + height - 2);
    glEnd();
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
    glColor3f(color.r, color.g, color.b);
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
    
    glBegin(GL_LINES);
    
    // Segment 0 (top)
    if (segments[0]) { glVertex2f(x, y); glVertex2f(x + w, y); }
    
    // Segment 1 (top-left)
    if (segments[1]) { glVertex2f(x, y); glVertex2f(x, y + h/2); }
    
    // Segment 2 (top-right)
    if (segments[2]) { glVertex2f(x + w, y); glVertex2f(x + w, y + h/2); }
    
    // Segment 3 (middle)
    if (segments[3]) { glVertex2f(x, y + h/2); glVertex2f(x + w, y + h/2); }
    
    // Segment 4 (bottom-left)
    if (segments[4]) { glVertex2f(x, y + h/2); glVertex2f(x, y + h); }
    
    // Segment 5 (bottom-right)
    if (segments[5]) { glVertex2f(x + w, y + h/2); glVertex2f(x + w, y + h); }
    
    // Segment 6 (bottom)
    if (segments[6]) { glVertex2f(x, y + h); glVertex2f(x + w, y + h); }
    
    glEnd();
    glLineWidth(1.0f);
}

void UIManager::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    // Заглушка для будущей реализации текста
    // Пока используем только цифры
}

void UIManager::Shutdown() {
    initialized_ = false;
}

