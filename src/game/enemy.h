#pragma once

#include <glm/glm.hpp>
#include <memory>

class Enemy {
public:
    Enemy();
    ~Enemy();

    // Initialize enemy with spawn position
    bool Initialize(const glm::vec3& spawn_position);

    // Update enemy logic
    void Update(float delta_time);

    // Render enemy
    void Render();

    // Getters
    glm::vec3 GetPosition() const { return position_; }
    glm::vec3 GetTargetPosition() const { return target_position_; }
    float GetSpeed() const { return speed_; }
    float GetHealth() const { return health_; }
    float GetMaxHealth() const { return max_health_; }
    bool IsAlive() const { return alive_; }
    glm::vec3 GetColor() const { return color_; }

    // Setters
    void SetTargetPosition(const glm::vec3& target) { target_position_ = target; }
    void SetSpeed(float speed) { speed_ = speed; }
    void SetHealth(float health) { health_ = health; }
    void SetColor(const glm::vec3& color) { color_ = color; }

    // Combat
    void TakeDamage(float damage);
    void Die();

    // Movement
    void MoveTowardsTarget(float delta_time);

private:
    glm::vec3 position_;        // Current position
    glm::vec3 target_position_; // Target position (center cube)
    float speed_;               // Movement speed
    float health_;              // Current health
    float max_health_;          // Maximum health
    glm::vec3 color_;           // Enemy color (red for enemies)
    bool alive_;                // Is enemy alive
    bool initialized_;          // Is enemy initialized

    // Movement calculations
    glm::vec3 direction_;       // Direction to target
    float distance_to_target_;  // Distance to target

    // Update movement direction
    void UpdateDirection();
};
