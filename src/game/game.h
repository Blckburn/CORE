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
class RayCaster;
class TurretPreview;
class ProjectileManager;
class WaveManager;
class UIManager;

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
    
    bool initialized_;
    
    // Turret placement state
    bool turret_placement_mode_;
    glm::vec3 preview_position_;
    bool preview_valid_;
};
