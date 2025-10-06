#pragma once

#include "turret.h"
#include "enemy.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class TurretManager {
public:
    TurretManager();
    ~TurretManager();

    // Initialize turret manager
    bool Initialize();

    // Update all turrets
    void Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void SetProjectileManager(class ProjectileManager* projectile_manager);

    // Render all turrets
    void Render();

    // Turret management
    bool PlaceTurret(const glm::vec3& position);
    void RemoveTurret(int index);
    void ClearAllTurrets();

    // Getters
    const std::vector<std::unique_ptr<Turret>>& GetTurrets() const { return turrets_; }
    int GetTurretCount() const { return static_cast<int>(turrets_.size()); }
    int GetActiveTurretCount() const;

    // Placement validation
    bool IsValidPlacement(const glm::vec3& position) const;
    float GetMinDistanceFromCenter() const { return min_distance_from_center_; }
    float GetMaxDistanceFromCenter() const { return max_distance_from_center_; }

    // Configuration
    void SetMinDistanceFromCenter(float distance) { min_distance_from_center_ = distance; }
    void SetMaxDistanceFromCenter(float distance) { max_distance_from_center_ = distance; }
    void SetMinDistanceBetweenTurrets(float distance) { min_distance_between_turrets_ = distance; }

private:
    std::vector<std::unique_ptr<Turret>> turrets_;
    
    // Placement constraints
    float min_distance_from_center_;     // Minimum distance from center cube
    float max_distance_from_center_;     // Maximum distance from center cube
    float min_distance_between_turrets_; // Minimum distance between turrets
    
    class ProjectileManager* projectile_manager_;
    
    // Helper functions
    float CalculateDistanceFromCenter(const glm::vec3& position) const;
    bool IsTooCloseToOtherTurrets(const glm::vec3& position) const;
    bool IsWithinPlacementBounds(const glm::vec3& position) const;
};
