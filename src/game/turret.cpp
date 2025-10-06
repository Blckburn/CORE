#include "turret.h"
#include "enemy.h"
#include "projectile_manager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <iostream>

Turret::Turret() :
    position_(0.0f),
    range_(15.0f),              // 15 unit range
    damage_(25.0f),             // 25 damage per shot
    fire_rate_(2.0f),           // 2 shots per second
    color_(0.0f, 1.0f, 0.0f),   // Green color for turrets
    active_(true),
    initialized_(false),
    current_target_(nullptr),
    rotation_(0.0f),
    target_rotation_(0.0f),
    rotation_speed_(180.0f),    // 180 degrees per second
    last_fire_time_(0.0f),
    reload_time_(0.0f) {
}

Turret::~Turret() {
}

bool Turret::Initialize(const glm::vec3& position) {
    std::cout << "Initializing turret at position: " 
              << position.x << ", " << position.y << ", " << position.z << std::endl;
    
    position_ = position;
    reload_time_ = 1.0f / fire_rate_;  // Calculate reload time from fire rate
    initialized_ = true;
    
    return true;
}

void Turret::Update(float delta_time) {
    if (!active_ || !initialized_) return;
    
    // Update fire timer
    UpdateFireTimer(delta_time);
    
    // Update rotation towards target
    UpdateRotation(delta_time);
    
    // Fire logic is now handled by TurretManager
    // No direct firing in Update() method
}

void Turret::Render() {
    if (!active_ || !initialized_) return;
    
    // TODO: Implement rendering
    // This will be called from the game render loop
}

void Turret::UpdateTarget(const std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!active_) return;
    
    // Clear current target if it's no longer valid
    if (current_target_ && (!current_target_->IsAlive() || !IsInRange(current_target_))) {
        current_target_ = nullptr;
    }
    
    // If we don't have a target, find the closest enemy in range
    if (!current_target_) {
        Enemy* closest_enemy = nullptr;
        float closest_distance = range_;
        
        for (const auto& enemy : enemies) {
            if (!enemy || !enemy->IsAlive()) continue;
            
            float distance = CalculateDistanceToTarget(enemy.get());
            if (distance <= range_ && distance < closest_distance) {
                closest_enemy = enemy.get();
                closest_distance = distance;
            }
        }
        
        if (closest_enemy) {
            current_target_ = closest_enemy;
            std::cout << "Turret acquired target at distance: " << closest_distance << std::endl;
        }
    }
}

void Turret::ClearTarget() {
    current_target_ = nullptr;
}

void Turret::Fire() {
    if (!current_target_ || !CanFire()) return;
    
    std::cout << "Turret fired at enemy!" << std::endl;
    
    // Deal damage to target
    current_target_->TakeDamage(damage_);
    
    // Update fire timing
    last_fire_time_ = 0.0f;  // Reset timer (will be updated by game loop)
    
    // Clear target if enemy died
    if (!current_target_->IsAlive()) {
        current_target_ = nullptr;
    }
}

void Turret::Fire(ProjectileManager* projectile_manager) {
    if (!current_target_ || !CanFire()) return;
    
    // Create projectile from turret position to target
    projectile_manager->CreateProjectile(position_, current_target_->GetPosition(), 30.0f, damage_);
    
    // Update fire timing
    last_fire_time_ = 0.0f;
    
    std::cout << "Turret fired projectile at enemy!" << std::endl;
}

bool Turret::CanFire() const {
    return last_fire_time_ >= reload_time_;
}

void Turret::UpdateFireTimer(float delta_time) {
    last_fire_time_ += delta_time;
}

void Turret::UpdateRotation(float delta_time) {
    if (!current_target_) {
        // No target - rotate to default position (0 degrees)
        target_rotation_ = 0.0f;
    } else {
        // Calculate rotation towards target
        glm::vec3 direction = GetDirectionToTarget();
        target_rotation_ = glm::degrees(glm::atan(direction.x, direction.z));
    }
    
    // Smoothly rotate towards target
    float rotation_diff = target_rotation_ - rotation_;
    
    // Normalize angle difference to [-180, 180]
    while (rotation_diff > 180.0f) rotation_diff -= 360.0f;
    while (rotation_diff < -180.0f) rotation_diff += 360.0f;
    
    // Rotate towards target
    float max_rotation = rotation_speed_ * delta_time;
    if (glm::abs(rotation_diff) <= max_rotation) {
        rotation_ = target_rotation_;
    } else {
        rotation_ += (rotation_diff > 0.0f ? max_rotation : -max_rotation);
    }
}

float Turret::CalculateDistanceToTarget(Enemy* enemy) const {
    if (!enemy) return 999999.0f;
    
    glm::vec3 enemy_pos = enemy->GetPosition();
    return glm::length(enemy_pos - position_);
}

bool Turret::IsInRange(Enemy* enemy) const {
    return CalculateDistanceToTarget(enemy) <= range_;
}

bool Turret::HasLineOfSight(Enemy* enemy) const {
    // For now, assume all enemies have line of sight
    // In a more complex game, you'd check for obstacles
    return true;
}

glm::vec3 Turret::GetDirectionToTarget() const {
    if (!current_target_) return glm::vec3(0.0f, 0.0f, 1.0f);
    
    glm::vec3 enemy_pos = current_target_->GetPosition();
    glm::vec3 direction = enemy_pos - position_;
    direction.y = 0.0f;  // Keep rotation only on Y axis (horizontal)
    
    if (glm::length(direction) > 0.001f) {
        return glm::normalize(direction);
    }
    
    return glm::vec3(0.0f, 0.0f, 1.0f);  // Default forward direction
}
