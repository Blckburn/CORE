#include "projectile.h"
#include <iostream>

Projectile::Projectile()
    : position_(0.0f), target_position_(0.0f), direction_(0.0f), speed_(0.0f), damage_(0),
      color_(0.0f, 1.0f, 1.0f), // Cyan color like in TRON
      initialized_(false), active_(false), has_hit_target_(false),
      lifetime_(3.0f), current_lifetime_(0.0f) {
}

Projectile::~Projectile() {
}

bool Projectile::Initialize(const glm::vec3& start_position, const glm::vec3& target_position, float speed, int damage) {
    position_ = start_position;
    target_position_ = target_position;
    speed_ = speed;
    damage_ = damage;
    
    // Calculate direction to target
    direction_ = target_position - start_position;
    float distance = glm::length(direction_);
    
    if (distance > 0.001f) {
        direction_ = glm::normalize(direction_);
    } else {
        // If target is at same position, use default direction
        direction_ = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    
    initialized_ = true;
    active_ = true;
    has_hit_target_ = false;
    current_lifetime_ = 0.0f;
    
    std::cout << "Projectile initialized from (" << start_position.x << ", " << start_position.y << ", " << start_position.z 
              << ") to (" << target_position.x << ", " << target_position.y << ", " << target_position.z << ")" << std::endl;
    
    return true;
}

void Projectile::Update(float delta_time) {
    if (!active_ || !initialized_) return;
    
    // Update lifetime
    current_lifetime_ += delta_time;
    
    // Check if projectile has expired
    if (current_lifetime_ >= lifetime_) {
        active_ = false;
        std::cout << "Projectile expired after " << current_lifetime_ << " seconds" << std::endl;
        return;
    }
    
    // Move projectile
    position_ += direction_ * speed_ * delta_time;
    
    // Check if projectile has reached target area
    float distance_to_target = glm::length(target_position_ - position_);
    if (distance_to_target < 1.0f) { // Hit radius
        has_hit_target_ = true;
        active_ = false;
        std::cout << "Projectile hit target at distance: " << distance_to_target << std::endl;
    }
}

void Projectile::Render() {
    // Rendering is handled by the Game class
}

void Projectile::SetTarget(const glm::vec3& target_position) {
    target_position_ = target_position;
    
    // Recalculate direction
    direction_ = target_position - position_;
    float distance = glm::length(direction_);
    
    if (distance > 0.001f) {
        direction_ = glm::normalize(direction_);
    }
}

void Projectile::UpdateTarget(const glm::vec3& new_target_position) {
    target_position_ = new_target_position;
    
    // Recalculate direction to new target
    direction_ = target_position_ - position_;
    float distance = glm::length(direction_);
    
    if (distance > 0.001f) {
        direction_ = glm::normalize(direction_);
    }
}

bool Projectile::CheckHit(const glm::vec3& target_position, float hit_radius) const {
    if (!active_) return false;
    
    float distance = glm::length(target_position - position_);
    return distance <= hit_radius;
}
