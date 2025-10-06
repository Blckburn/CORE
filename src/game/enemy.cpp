#include "enemy.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/random.hpp>
#include <iostream>

Enemy::Enemy() : 
    position_(0.0f),
    target_position_(0.0f),
    speed_(5.0f),
    health_(100.0f),
    max_health_(100.0f),
    color_(1.0f, 0.0f, 0.0f), // Red color for enemies
    alive_(false),
    initialized_(false),
    direction_(0.0f),
    distance_to_target_(0.0f) {
}

Enemy::~Enemy() {
}

bool Enemy::Initialize(const glm::vec3& spawn_position) {
    std::cout << "Initializing enemy at position: " 
              << spawn_position.x << ", " << spawn_position.y << ", " << spawn_position.z << std::endl;
    
    position_ = spawn_position;
    target_position_ = glm::vec3(0.0f, 0.0f, 0.0f); // Center cube position
    alive_ = true;
    initialized_ = true;
    
    // Update direction to target
    UpdateDirection();
    
    return true;
}

void Enemy::Update(float delta_time) {
    if (!alive_ || !initialized_) return;
    
    // Move towards target
    MoveTowardsTarget(delta_time);
    
    // Check if reached target (center cube)
    if (distance_to_target_ < 1.0f) {
        std::cout << "Enemy reached center cube!" << std::endl;
        Die();
    }
}

void Enemy::Render() {
    if (!alive_ || !initialized_) return;
    
    // TODO: Implement rendering
    // This will be called from the game render loop
}

void Enemy::TakeDamage(float damage) {
    if (!alive_) return;
    
    health_ -= damage;
    std::cout << "Enemy took " << damage << " damage. Health: " << health_ << "/" << max_health_ << std::endl;
    
    if (health_ <= 0.0f) {
        Die();
    }
}

void Enemy::Die() {
    if (!alive_) return;
    
    alive_ = false;
    std::cout << "Enemy died at position: " 
              << position_.x << ", " << position_.y << ", " << position_.z << std::endl;
}

void Enemy::MoveTowardsTarget(float delta_time) {
    if (!alive_) return;
    
    // Update direction to target
    UpdateDirection();
    
    // Move towards target
    glm::vec3 movement = direction_ * speed_ * delta_time;
    position_ += movement;
    
    // Update distance to target
    distance_to_target_ = glm::length(target_position_ - position_);
}

void Enemy::UpdateDirection() {
    glm::vec3 to_target = target_position_ - position_;
    distance_to_target_ = glm::length(to_target);
    
    if (distance_to_target_ > 0.001f) {
        direction_ = glm::normalize(to_target);
    } else {
        direction_ = glm::vec3(0.0f);
    }
}
