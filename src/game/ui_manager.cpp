#include "ui_manager.h"
#include "wave_manager.h"
#include "graphics/shader.h"
#include "graphics/font.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

UIManager::UIManager()
    : shader_(nullptr)
    , text_shader_(nullptr)
    , font_(nullptr)
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
    
    // Загружаем текстовый шейдер
    text_shader_ = new Shader();
    
    // Пробуем разные пути к текстовым шейдерам
    std::vector<std::string> shader_paths = {
        "assets/shaders/",
        "../assets/shaders/",
        "../../assets/shaders/",
        "build/assets/shaders/"
    };
    
    bool text_shader_loaded = false;
    for (const auto& path : shader_paths) {
        std::string vs_path = path + "text.vs";
        std::string fs_path = path + "text.fs";
        if (text_shader_->LoadFromFiles(vs_path, fs_path)) {
            std::cout << "Text shader loaded successfully from: " << path << std::endl;
            text_shader_loaded = true;
            break;
        }
    }
    
    if (!text_shader_loaded) {
        std::cout << "UIManager::Initialize: Failed to load text shader from any path" << std::endl;
        delete text_shader_;
        text_shader_ = nullptr;
        return false;
    }
    
    // Загружаем шрифт
    font_ = new Font();
    
    // Пробуем разные пути к шрифту
    std::vector<std::string> font_paths = {
        "assets/fonts/RobotoMono-Regular.ttf",
        "../assets/fonts/RobotoMono-Regular.ttf",
        "../../assets/fonts/RobotoMono-Regular.ttf",
        "build/assets/fonts/RobotoMono-Regular.ttf"
    };
    
    std::cout << "Attempting to load font from different paths:" << std::endl;
    bool font_loaded = false;
    for (const auto& path : font_paths) {
        std::cout << "  Trying path: " << path << "... ";
        if (font_->LoadFont(path, 32)) {
            std::cout << "SUCCESS!" << std::endl;
            font_loaded = true;
            break;
        } else {
            std::cout << "FAILED" << std::endl;
        }
    }
    
    if (!font_loaded) {
        std::cout << "UIManager::Initialize: Failed to load font from any path" << std::endl;
        delete font_;
        font_ = nullptr;
        delete text_shader_;
        text_shader_ = nullptr;
        return false;
    }
    
    initialized_ = true;
    
    std::cout << "UI Manager initialized successfully with font!" << std::endl;
    return true;
}

void UIManager::Render(WaveManager* wave_manager, int window_width, int window_height) {
    if (!initialized_ || !wave_manager || !shader_) return;
    
    // Обновляем размеры viewport (КРИТИЧЕСКИ ВАЖНО!)
    viewport_width_ = window_width;
    viewport_height_ = window_height;
    
    // Сохраняем текущее состояние
    GLboolean depth_test_enabled = glIsEnabled(GL_DEPTH_TEST);
    
    // Отключаем depth test для UI
    glDisable(GL_DEPTH_TEST);
    
    // Используем наш shader
    shader_->Use();
    
    // Создаём ортографическую проекцию для 2D UI
    glm::mat4 projection = glm::ortho(0.0f, (float)viewport_width_, (float)viewport_height_, 0.0f, -1.0f, 1.0f);
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
    float ui_spacing = 60.0f;
    glm::vec3 cyan_color(0.0f, 1.0f, 1.0f);
    glm::vec3 green_color(0.0f, 1.0f, 0.0f);
    glm::vec3 red_color(1.0f, 0.0f, 0.0f);
    glm::vec3 yellow_color(1.0f, 1.0f, 0.0f);
    glm::vec3 white_color(1.0f, 1.0f, 1.0f);
    
    // Wave number with label
    RenderText("WAVE", 20.0f, ui_y, 1.0f, cyan_color);
    RenderNumber(current_wave, 20.0f, ui_y + 25.0f, 1.0f, cyan_color);
    ui_y += ui_spacing;
    
    // Score with label
    RenderText("SCORE", 20.0f, ui_y, 1.0f, green_color);
    RenderNumber(score, 20.0f, ui_y + 25.0f, 1.0f, green_color);
    ui_y += ui_spacing;

    // Currency
    int money = wave_manager->GetCurrency();
    RenderText("CREDITS", 20.0f, ui_y, 1.0f, white_color);
    RenderNumber(money, 20.0f, ui_y + 25.0f, 1.0f, white_color);
    ui_y += ui_spacing;

    // Стоимость башни показываем только в тултипе у курсора (см. Game::Render)
    
    // Time or enemies remaining with label
    if (!is_wave_active && time_till_wave > 0.0f) {
        RenderText("NEXT", 20.0f, ui_y, 1.0f, yellow_color);
        int seconds = static_cast<int>(time_till_wave);
        RenderNumber(seconds, 20.0f, ui_y + 25.0f, 1.0f, yellow_color);
    } else {
        RenderText("ENEMIES", 20.0f, ui_y, 1.0f, red_color);
        int enemies = wave_manager->GetEnemiesRemaining();
        RenderNumber(enemies, 20.0f, ui_y + 25.0f, 1.0f, red_color);
    }
    
    // Health bar with label
    float health_percent = wave_manager->GetCoreHealth() / 100.0f; // Получаем реальное здоровье
    RenderText("CORE", window_width - 220.0f, 20.0f, 1.0f, cyan_color);
    RenderBar(window_width - 220.0f, 45.0f, 200.0f, 20.0f, health_percent, cyan_color);
    
    // Game status
    if (wave_manager->IsGameOver()) {
        RenderText("GAME OVER", window_width/2 - 60.0f, window_height/2 - 20.0f, 2.0f, red_color);
    } else if (!is_wave_active && time_till_wave > 0.0f) {
        RenderText("PREPARING...", window_width/2 - 80.0f, window_height/2 - 20.0f, 1.5f, yellow_color);
    }
    
    // Восстанавливаем состояние
    if (depth_test_enabled) {
        glEnable(GL_DEPTH_TEST);
    }
}

void UIManager::RenderPausedOverlay(int window_width, int window_height) {
    if (!font_ || !text_shader_) return;
    viewport_width_ = window_width;
    viewport_height_ = window_height;
    GLboolean depth_enabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    text_shader_->Use();
    glm::mat4 projection = glm::ortho(0.0f, (float)viewport_width_, (float)viewport_height_, 0.0f);
    text_shader_->SetUniform("projection", projection);
    text_shader_->SetUniform("text_color", glm::vec3(1.0f, 1.0f, 1.0f));
    text_shader_->SetUniform("text", 0);
    font_->RenderText("PAUSED", window_width/2 - 80.0f, window_height/2 - 20.0f, 1.5f, glm::vec3(1.0f));
    if (depth_enabled) glEnable(GL_DEPTH_TEST);
}

void UIManager::RenderTooltip(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    if (!font_ || !text_shader_) return;
    
    // DEBUG: Print tooltip coords occasionally
    static int tooltip_debug_counter = 0;
    if (++tooltip_debug_counter % 60 == 0) {
        std::cout << "DEBUG TOOLTIP: viewport=" << viewport_width_ << "x" << viewport_height_ 
                  << ", pos=" << x << "," << y << ", text=\"" << text << "\"" << std::endl;
    }
    
    GLboolean depth_enabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    text_shader_->Use();
    glm::mat4 projection = glm::ortho(0.0f, (float)viewport_width_, (float)viewport_height_, 0.0f);
    text_shader_->SetUniform("projection", projection);
    text_shader_->SetUniform("text_color", color);
    text_shader_->SetUniform("text", 0);
    font_->RenderText(text, x, y, scale, color);
    if (depth_enabled) glEnable(GL_DEPTH_TEST);
}

void UIManager::RenderMainMenu(int window_width, int window_height, int selected_index) {
    if (!font_ || !text_shader_) return;
    viewport_width_ = window_width;
    viewport_height_ = window_height;
    GLboolean depth_enabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    // Dim background
    RenderDimBackground(window_width, window_height, 0.4f);
    text_shader_->Use();
    glm::mat4 projection = glm::ortho(0.0f, (float)viewport_width_, (float)viewport_height_, 0.0f);
    text_shader_->SetUniform("projection", projection);
    text_shader_->SetUniform("text", 0);
    float cx = window_width / 2.0f - 100.0f;
    float cy = window_height / 2.0f - 60.0f;
    const char* items[3] = {"START GAME", "OPTIONS", "EXIT"};
    for (int i = 0; i < 3; ++i) {
        glm::vec3 c = (i == selected_index) ? glm::vec3(1.0f, 1.0f, 0.0f) : glm::vec3(1.0f);
        text_shader_->SetUniform("text_color", c);
        font_->RenderText(items[i], cx, cy + i * 30.0f, 1.0f, c);
    }
    if (depth_enabled) glEnable(GL_DEPTH_TEST);
}

void UIManager::RenderOptionsMenu(int window_width, int window_height, int selected_index) {
    if (!font_ || !text_shader_) return;
    viewport_width_ = window_width;
    viewport_height_ = window_height;
    GLboolean depth_enabled = glIsEnabled(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    // Dim background
    RenderDimBackground(window_width, window_height, 0.4f);
    text_shader_->Use();
    glm::mat4 projection = glm::ortho(0.0f, (float)viewport_width_, (float)viewport_height_, 0.0f);
    text_shader_->SetUniform("projection", projection);
    text_shader_->SetUniform("text", 0);
    float cx = window_width / 2.0f - 140.0f;
    float cy = window_height / 2.0f - 90.0f;
    const char* items[4] = {"1280x720", "1920x1080", "2560x1440", "3840x2160"};
    for (int i = 0; i < 4; ++i) {
        glm::vec3 c = (i == selected_index) ? glm::vec3(0.0f, 1.0f, 1.0f) : glm::vec3(1.0f);
        text_shader_->SetUniform("text_color", c);
        font_->RenderText(items[i], cx, cy + i * 30.0f, 1.0f, c);
    }
    text_shader_->SetUniform("text_color", glm::vec3(1.0f, 1.0f, 0.0f));
    font_->RenderText("ENTER: APPLY, ESC: BACK", cx, cy + 4 * 30.0f + 20.0f, 0.8f, glm::vec3(1.0f,1.0f,0.0f));
    if (depth_enabled) glEnable(GL_DEPTH_TEST);
}

void UIManager::RenderDimBackground(int window_width, int window_height, float alpha) {
    // Simple full-screen quad with shader_ color
    if (!shader_) return;
    shader_->Use();
    glm::mat4 projection = glm::ortho(0.0f, (float)window_width, (float)window_height, 0.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    shader_->SetUniform("projection", projection);
    shader_->SetUniform("view", view);
    shader_->SetUniform("model", model);
    shader_->SetUniform("color", glm::vec3(0.0f, 0.0f, 0.0f));
    float vertices[] = {
        0.0f, 0.0f,
        (float)window_width, 0.0f,
        (float)window_width, (float)window_height,
        0.0f, 0.0f,
        (float)window_width, (float)window_height,
        0.0f, (float)window_height
    };
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Use alpha via OpenGL blend state already enabled globally; color alpha simulated via shader color intensity on wireframe shader
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
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
    // Преобразуем число в строку и используем настоящий шрифт
    std::string number_str = std::to_string(number);
    RenderText(number_str, x, y, scale, color);
}

void UIManager::RenderDigit(int digit, float x, float y, float scale, const glm::vec3& color) {
    // Преобразуем цифру в строку и используем настоящий шрифт
    std::string digit_str = std::to_string(digit);
    RenderText(digit_str, x, y, scale, color);
}

void UIManager::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    if (!font_ || !text_shader_) return;
    
    // Используем текстовый шейдер
    text_shader_->Use();
    
    // Настраиваем шейдер для текста (используем реальный размер viewport)
    glm::mat4 projection = glm::ortho(0.0f, (float)viewport_width_, (float)viewport_height_, 0.0f);
    text_shader_->SetUniform("projection", projection);
    text_shader_->SetUniform("text_color", color);
    
    // Устанавливаем uniform для текстуры (КРИТИЧЕСКИ ВАЖНО!)
    text_shader_->SetUniform("text", 0);
    
    // Рендерим текст через Font
    font_->RenderText(text, x, y, scale, color);
}

void UIManager::Shutdown() {
    if (font_) {
        font_->Shutdown();
        delete font_;
        font_ = nullptr;
    }
    if (text_shader_) {
        delete text_shader_;
        text_shader_ = nullptr;
    }
    initialized_ = false;
}

