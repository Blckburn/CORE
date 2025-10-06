#include "turret_manager.h"
#include <algorithm>
#include <iostream>

TurretManager::TurretManager() :
    min_distance_from_center_(5.0f),      // At least 5 units from center
    max_distance_from_center_(20.0f),     // At most 20 units from center
    min_distance_between_turrets_(3.0f),  // At least 3 units between turrets
    projectile_manager_(nullptr) {
}

TurretManager::~TurretManager() {
    turrets_.clear();
}

bool TurretManager::Initialize() {
    std::cout << "Initializing turret manager..." << std::endl;
    std::cout << "Min distance from center: " << min_distance_from_center_ << std::endl;
    std::cout << "Max distance from center: " << max_distance_from_center_ << std::endl;
    std::cout << "Min distance between turrets: " << min_distance_between_turrets_ << std::endl;
    
    return true;
}

void TurretManager::Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies) {
    // Update all turrets
    for (auto& turret : turrets_) {
        if (turret && turret->IsActive()) {
            // Update targeting for this turret
            turret->UpdateTarget(enemies);
            
            // Update turret logic
            turret->Update(delta_time);
            
            // Fire projectiles if turret can fire
            if (turret->CanFire() && turret->GetCurrentTarget() && projectile_manager_) {
                std::cout << "TurretManager: Turret can fire, calling Fire()" << std::endl;
                turret->Fire(projectile_manager_);
            } else {
                if (!turret->CanFire()) {
                    // std::cout << "TurretManager: Turret cannot fire yet" << std::endl;
                }
                if (!turret->GetCurrentTarget()) {
                    // std::cout << "TurretManager: Turret has no target" << std::endl;
                }
                if (!projectile_manager_) {
                    std::cout << "TurretManager: ProjectileManager is null!" << std::endl;
                }
            }
        }
    }
}

void TurretManager::SetProjectileManager(class ProjectileManager* projectile_manager) {
    projectile_manager_ = projectile_manager;
}

void TurretManager::Render() {
    // Render all active turrets
    for (auto& turret : turrets_) {
        if (turret && turret->IsActive()) {
            turret->Render();
        }
    }
}

bool TurretManager::PlaceTurret(const glm::vec3& position) {
    std::cout << "Attempting to place turret at: " 
              << position.x << ", " << position.y << ", " << position.z << std::endl;
    
    // Validate placement
    if (!IsValidPlacement(position)) {
        std::cout << "Invalid turret placement at: " 
                  << position.x << ", " << position.y << ", " << position.z << std::endl;
        return false;
    }
    
    // Create new turret
    auto turret = std::make_unique<Turret>();
    if (turret->Initialize(position)) {
        turrets_.push_back(std::move(turret));
        std::cout << "Turret placed successfully at: " 
                  << position.x << ", " << position.y << ", " << position.z << std::endl;
        std::cout << "Total turrets: " << turrets_.size() << std::endl;
        return true;
    }
    
    return false;
}

void TurretManager::RemoveTurret(int index) {
    if (index >= 0 && index < static_cast<int>(turrets_.size())) {
        std::cout << "Removing turret at index: " << index << std::endl;
        turrets_.erase(turrets_.begin() + index);
    }
}

void TurretManager::ClearAllTurrets() {
    std::cout << "Clearing all turrets (" << turrets_.size() << " turrets)" << std::endl;
    turrets_.clear();
}

int TurretManager::GetActiveTurretCount() const {
    int count = 0;
    for (const auto& turret : turrets_) {
        if (turret && turret->IsActive()) {
            count++;
        }
    }
    return count;
}

bool TurretManager::IsValidPlacement(const glm::vec3& position) const {
    // Check if position is within placement bounds
    if (!IsWithinPlacementBounds(position)) {
        return false;
    }
    
    // Check if position is too close to other turrets
    if (IsTooCloseToOtherTurrets(position)) {
        return false;
    }
    
    return true;
}

float TurretManager::CalculateDistanceFromCenter(const glm::vec3& position) const {
    return glm::length(position);  // Center is at origin (0,0,0)
}

bool TurretManager::IsTooCloseToOtherTurrets(const glm::vec3& position) const {
    for (const auto& turret : turrets_) {
        if (!turret) continue;
        
        glm::vec3 turret_pos = turret->GetPosition();
        float distance = glm::length(position - turret_pos);
        
        if (distance < min_distance_between_turrets_) {
            return true;
        }
    }
    return false;
}

bool TurretManager::IsWithinPlacementBounds(const glm::vec3& position) const {
    float distance_from_center = CalculateDistanceFromCenter(position);
    
    return distance_from_center >= min_distance_from_center_ && 
           distance_from_center <= max_distance_from_center_;
}
