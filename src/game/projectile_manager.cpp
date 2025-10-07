#include "projectile_manager.h"
#include "projectile.h"
#include "enemy.h"
#include "wave_manager.h"
#include <iostream>
#include <limits>

ProjectileManager::ProjectileManager()
    : wave_manager_(nullptr)
    , default_speed_(30.0f)
    , default_damage_(3)  // Снижено примерно в 10 раз (25 -> 3)
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
            
            // Semi-homing: while enemy alive, projectile tracks it; if enemy died,
            // projectile keeps last known target position and finishes flight.
            if (projectile->HasHitTarget()) {
                bool damaged = false;
                for (const auto& enemy : enemies) {
                    if (enemy && enemy->IsAlive()) {
                        if (projectile->CheckHit(enemy->GetPosition(), 1.2f)) {
                            glm::vec3 enemy_pos = enemy->GetPosition(); // Save position before damage
                            enemy->TakeDamage(projectile->GetDamage());
                            std::cout << "Projectile hit enemy for " << projectile->GetDamage() << " damage!" << std::endl;
                            if (!enemy->IsAlive() && wave_manager_) {
                                wave_manager_->OnEnemyDestroyed(enemy_pos); // Pass enemy death position
                            }
                            damaged = true;
                            break;
                        }
                    }
                }
                // Deactivate regardless of whether we damaged something
                projectile->SetActive(false);
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

void ProjectileManager::CreateProjectile(const glm::vec3& start_position, const glm::vec3& target_position, float speed, int damage, Enemy* target_enemy) {
    auto projectile = std::make_unique<Projectile>();
    if (projectile->Initialize(start_position, target_position, speed, damage, target_enemy)) {
        projectiles_.push_back(std::move(projectile));
        std::cout << "Created projectile #" << projectiles_.size() 
                  << " from (" << start_position.x << ", " << start_position.y << ", " << start_position.z << ")"
                  << " to (" << target_position.x << ", " << target_position.y << ", " << target_position.z << ")" << std::endl;
    }
}
