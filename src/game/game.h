// Main game logic coordinator and state manager
#pragma once

#include <memory>
#include <glm/glm.hpp>

class Renderer;
class InputManager;
class Shader;
class Mesh;
class Camera;
class EnemySpawner;
class TurretManager;
class Turret;
class RayCaster;
class TurretPreview;
class ProjectileManager;
class WaveManager;
class UIManager;
class ItemManager;
class Item;

class Game {
public:
    Game();
    ~Game();
    
    bool Initialize(Renderer* renderer, InputManager* input);
    void Update();
    void Render();
    void Shutdown();
    
private:
    Renderer* renderer_;
    InputManager* input_;
    
    // Graphics objects
    std::unique_ptr<Shader> shader_;
    std::unique_ptr<Mesh> cube_mesh_;
    std::unique_ptr<Mesh> enemy_mesh_;
    std::unique_ptr<Mesh> turret_mesh_;
    std::unique_ptr<Mesh> projectile_mesh_;
    std::unique_ptr<Camera> camera_;
    
    // Game systems
    std::unique_ptr<EnemySpawner> enemy_spawner_;
    std::unique_ptr<TurretManager> turret_manager_;
    std::unique_ptr<RayCaster> ray_caster_;
    std::unique_ptr<TurretPreview> turret_preview_;
    std::unique_ptr<ProjectileManager> projectile_manager_;
    std::unique_ptr<WaveManager> wave_manager_;
    std::unique_ptr<UIManager> ui_manager_;
    std::unique_ptr<ItemManager> item_manager_;
    
    bool initialized_;
    
    // Turret placement state
    bool turret_placement_mode_;
    glm::vec3 preview_position_;
    bool preview_valid_;
    
    // Turret management state
    Turret* selected_turret_;
    Turret* hovered_turret_;
    bool turret_menu_open_;
    glm::vec3 turret_menu_position_;
    
    // Item management state
    Item* hovered_item_;
    int selected_inventory_index_; // -1 = nothing selected

    // Economy
    int turret_cost_ = 1; // Начальная стоимость первой башни

    // Pause state
    bool paused_ = false;

    // Game state / menus
    enum class GameState { MainMenu, Options, Playing, Paused, GameOver };
    GameState state_ = GameState::MainMenu;
    int main_menu_index_ = 0;
    int options_selected_index_ = 1; // default 1920x1080
    int game_over_menu_index_ = 0; // 0 = Restart, 1 = Main Menu
    bool inventory_open_ = false; // I key toggles inventory
};
