#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "projectile.h"
#include "enemy.h"

class WaveManager;

class ProjectileManager {
public:
    ProjectileManager();
    ~ProjectileManager();

    bool Initialize();
    void Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void Render(); // Placeholder, actual rendering in Game class

    // Projectile creation
    void CreateProjectile(const glm::vec3& start_position, const glm::vec3& target_position, float speed, int damage);
    
    // Wave manager integration
    void SetWaveManager(WaveManager* wave_manager) { wave_manager_ = wave_manager; }
    
    // Getters
    const std::vector<std::unique_ptr<Projectile>>& GetProjectiles() const { return projectiles_; }
    int GetProjectileCount() const { return projectiles_.size(); }

private:
    std::vector<std::unique_ptr<Projectile>> projectiles_;
    WaveManager* wave_manager_;
    
    // Projectile properties
    float default_speed_;
    int default_damage_;
    glm::vec3 default_color_;
};
