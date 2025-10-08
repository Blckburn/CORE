// Defensive turret that auto-targets and shoots enemies
#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <array>

class Enemy;
class ProjectileManager;
class Item;

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
    float GetBaseDamage() const { return base_damage_; }
    float GetBaseFireRate() const { return base_fire_rate_; }
    float GetBaseRange() const { return base_range_; }
    bool IsActive() const { return active_; }
    glm::vec3 GetColor() const { return color_; }
    float GetRotation() const { return rotation_; }
    Enemy* GetCurrentTarget() const { return current_target_; }
    int GetCost() const { return cost_; }
    const std::array<Item*, 3>& GetItemSlots() const { return item_slots_; }

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
    void ResetFireTimer(); // Reset fire timer (for pause/unpause)

    // Visual
    void UpdateRotation(float delta_time);
    
    // Item management
    bool EquipItem(Item* item, int slot_index); // Equip item to slot (0-2)
    void RecalculateStats(); // Recalculate stats based on equipped items
    int GetEquippedItemCount() const; // Get number of equipped items

private:
    glm::vec3 position_;        // Turret position
    float range_;               // Attack range (modified by items)
    float damage_;              // Damage per shot (modified by items)
    float fire_rate_;           // Shots per second (modified by items)
    float base_range_;          // Base range (without items)
    float base_damage_;         // Base damage (without items)
    float base_fire_rate_;      // Base fire rate (without items)
    glm::vec3 color_;           // Turret color (green for turrets)
    bool active_;               // Is turret active
    bool initialized_;          // Is turret initialized
    int cost_;                  // Cost when placed (for sell refund)
    std::array<Item*, 3> item_slots_; // 3 slots for items

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
