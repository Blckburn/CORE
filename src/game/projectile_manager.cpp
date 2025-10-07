#include "projectile_manager.h"
#include "projectile.h"
#include "enemy.h"
#include "wave_manager.h"
#include <iostream>
#include <limits>

ProjectileManager::ProjectileManager()
    : wave_manager_(nullptr)
    , default_speed_(30.0f)
    , default_damage_(25)
    , default_color_(0.0f, 1.0f, 1.0f) {
}

ProjectileManager::~ProjectileManager() {
}

bool ProjectileManager::Initialize() {
    std::cout << "Initializing projectile manager..." << std::endl;
    std::cout << "Default projectile speed: " << default_speed_ << std::endl;
    std::cout << "Default projectile damage: " << default_damage_ << std::endl;
    return true;
}

void ProjectileManager::Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Update existing projectiles
    for (auto it = projectiles_.begin(); it != projectiles_.end();) {
        auto& projectile = *it;
        
        if (projectile && projectile->IsActive()) {
            projectile->Update(delta_time);
            
            // Update projectile target to closest enemy (homing missiles)
            if (!enemies.empty()) {
                float closest_distance = std::numeric_limits<float>::max();
                glm::vec3 closest_enemy_pos = glm::vec3(0.0f);
                
                for (const auto& enemy : enemies) {
                    if (enemy && enemy->IsAlive()) {
                        float distance = glm::length(enemy->GetPosition() - projectile->GetPosition());
                        if (distance < closest_distance) {
                            closest_distance = distance;
                            closest_enemy_pos = enemy->GetPosition();
                        }
                    }
                }
                
                // Update projectile direction to closest enemy
                if (closest_distance < std::numeric_limits<float>::max()) {
                    projectile->UpdateTarget(closest_enemy_pos);
                }
            }
            
            // Check collision with enemies
            for (const auto& enemy : enemies) {
                if (enemy && enemy->IsAlive()) {
                    if (projectile->CheckHit(enemy->GetPosition(), 1.0f)) {
                        enemy->TakeDamage(projectile->GetDamage());
                        projectile->SetActive(false); // Mark projectile as inactive
                        std::cout << "Projectile hit enemy for " << projectile->GetDamage() << " damage!" << std::endl;
                        
                        // Notify wave manager if enemy is destroyed
                        if (!enemy->IsAlive() && wave_manager_) {
                            wave_manager_->OnEnemyDestroyed();
                        }
                        break;
                    }
                }
            }
            
            ++it;
        } else {
            // Remove inactive projectiles
            it = projectiles_.erase(it);
        }
    }
}

void ProjectileManager::Render() {
    // Rendering is handled by the Game class
}

void ProjectileManager::CreateProjectile(const glm::vec3& start_position, const glm::vec3& target_position, float speed, int damage) {
    auto projectile = std::make_unique<Projectile>();
    if (projectile->Initialize(start_position, target_position, speed, damage)) {
        projectiles_.push_back(std::move(projectile));
        std::cout << "Created projectile #" << projectiles_.size() 
                  << " from (" << start_position.x << ", " << start_position.y << ", " << start_position.z << ")"
                  << " to (" << target_position.x << ", " << target_position.y << ", " << target_position.z << ")" << std::endl;
    }
}
