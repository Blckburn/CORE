#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Enemy;
class ProjectileManager;

class Turret {
public:
    Turret();
    ~Turret();

    // Initialize turret at position
    bool Initialize(const glm::vec3& position);

    // Update turret logic
    void Update(float delta_time);

    // Render turret
    void Render();

    // Getters
    glm::vec3 GetPosition() const { return position_; }
    float GetRange() const { return range_; }
    float GetDamage() const { return damage_; }
    float GetFireRate() const { return fire_rate_; }
    bool IsActive() const { return active_; }
    glm::vec3 GetColor() const { return color_; }
    float GetRotation() const { return rotation_; }
    Enemy* GetCurrentTarget() const { return current_target_; }
    int GetCost() const { return cost_; }

    // Setters
    void SetPosition(const glm::vec3& position) { position_ = position; }
    void SetRange(float range) { range_ = range; }
    void SetDamage(float damage) { damage_ = damage; }
    void SetFireRate(float rate) { fire_rate_ = rate; }
    void SetColor(const glm::vec3& color) { color_ = color; }
    void SetActive(bool active) { active_ = active; }
    void SetCost(int cost) { cost_ = cost; }

    // Targeting
    void UpdateTarget(const std::vector<std::unique_ptr<Enemy>>& enemies);
    void ClearTarget();

    // Combat
    void Fire();
    void Fire(ProjectileManager* projectile_manager);
    bool CanFire() const;
    void UpdateFireTimer(float delta_time);

    // Visual
    void UpdateRotation(float delta_time);

private:
    glm::vec3 position_;        // Turret position
    float range_;               // Attack range
    float damage_;              // Damage per shot
    float fire_rate_;           // Shots per second
    glm::vec3 color_;           // Turret color (green for turrets)
    bool active_;               // Is turret active
    bool initialized_;          // Is turret initialized
    int cost_;                  // Cost when placed (for sell refund)

    // Targeting
    Enemy* current_target_;     // Current target enemy
    float rotation_;            // Turret rotation angle (Y-axis)
    float target_rotation_;     // Target rotation angle
    float rotation_speed_;      // Rotation speed in degrees per second

    // Combat timing
    float last_fire_time_;      // Time of last shot
    float reload_time_;         // Time between shots

    // Helper functions
    float CalculateDistanceToTarget(Enemy* enemy) const;
    bool IsInRange(Enemy* enemy) const;
    bool HasLineOfSight(Enemy* enemy) const;
    glm::vec3 GetDirectionToTarget() const;
};
