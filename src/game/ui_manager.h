#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader;
class WaveManager;
class Font;

class UIManager {
public:
    UIManager();
    ~UIManager();

    bool Initialize(Shader* shader);
    void Render(WaveManager* wave_manager, int window_width, int window_height);
    void RenderWithTurrets(WaveManager* wave_manager, class TurretManager* turret_manager, int window_width, int window_height);
    void RenderPausedOverlay(int window_width, int window_height);
    void RenderTooltip(const std::string& text, float x, float y, float scale, const glm::vec3& color);
    void RenderTurretMenu(class Turret* turret, class Camera* camera, class InputManager* input, class ItemManager* item_manager, int selected_inventory_index, int window_width, int window_height, bool& sell_clicked, int& slot_clicked, int& inventory_clicked);
    void RenderInventoryScreen(class ItemManager* item_manager, int window_width, int window_height);
    
    // Render item grid on right side (for turret menu)
    void RenderItemGrid(class ItemManager* item_manager, class InputManager* input, int selected_index, 
                        int window_width, int window_height, int& clicked_item_index);
    // Menus
    void RenderMainMenu(int window_width, int window_height, int selected_index);
    void RenderOptionsMenu(int window_width, int window_height, int selected_index);
    void RenderGameOverMenu(int window_width, int window_height, int selected_index, WaveManager* wave_manager);
    void RenderDimBackground(int window_width, int window_height, float alpha);
    void Shutdown();

private:
    Shader* shader_;
    Shader* text_shader_;
    Font* font_;
    bool initialized_;
    int viewport_width_ = 1280;
    int viewport_height_ = 720;
    
    // Текстовый рендеринг через FreeType
    void RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color);
    void RenderDigit(int digit, float x, float y, float scale, const glm::vec3& color);
    void RenderNumber(int number, float x, float y, float scale, const glm::vec3& color);
    void RenderBar(float x, float y, float width, float height, float fill_percent, const glm::vec3& color);
    void RenderCurrency(int amount, float x, float y);
};

