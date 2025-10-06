#pragma once

#include <glm/glm.hpp>
#include <memory>

class Projectile {
public:
    Projectile();
    ~Projectile();

    bool Initialize(const glm::vec3& start_position, const glm::vec3& target_position, float speed, int damage);
    void Update(float delta_time);
    void Render(); // Placeholder, actual rendering in Game class

    // Getters
    const glm::vec3& GetPosition() const { return position_; }
    const glm::vec3& GetColor() const { return color_; }
    bool IsActive() const { return active_; }
    bool HasHitTarget() const { return has_hit_target_; }
    int GetDamage() const { return damage_; }

    // Target management
    void SetTarget(const glm::vec3& target_position);
    void UpdateTarget(const glm::vec3& new_target_position);
    bool CheckHit(const glm::vec3& target_position, float hit_radius) const;
    void SetActive(bool active) { active_ = active; }

private:
    glm::vec3 position_;
    glm::vec3 target_position_;
    glm::vec3 direction_;
    float speed_;
    int damage_;
    glm::vec3 color_;
    
    bool initialized_;
    bool active_;
    bool has_hit_target_;
    
    float lifetime_; // Maximum time before projectile disappears
    float current_lifetime_;
};
