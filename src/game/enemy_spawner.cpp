#include "enemy_spawner.h"
#include "wave_manager.h"
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <iostream>

EnemySpawner::EnemySpawner() :
    wave_manager_(nullptr),
    spawning_enabled_(false),
    spawn_rate_(1.0f),          // 1 enemy per second
    spawn_radius_(25.0f),       // 25 units from center
    time_since_last_spawn_(0.0f),
    gen_(rd_()),
    angle_dist_(0.0f, 2.0f * glm::pi<float>()),
    height_dist_(-spawn_radius_ * 0.5f, spawn_radius_ * 0.5f) {
}

EnemySpawner::~EnemySpawner() {
    enemies_.clear();
}

bool EnemySpawner::Initialize() {
    std::cout << "Initializing enemy spawner..." << std::endl;
    std::cout << "Spawn rate: " << spawn_rate_ << " enemies/second" << std::endl;
    std::cout << "Spawn radius: " << spawn_radius_ << " units" << std::endl;
    
    return true;
}

void EnemySpawner::Update(float delta_time) {
    // Update spawn timer
    UpdateSpawnTimer(delta_time);
    
    // Update all enemies
    for (auto& enemy : enemies_) {
        if (enemy && enemy->IsAlive()) {
            enemy->Update(delta_time);
            
            // Check if enemy reached core and notify wave manager
            if (enemy->HasReachedCore() && wave_manager_) {
                wave_manager_->OnEnemyReachedCore();
            }
        }
    }
    
    // Clean up dead enemies periodically
    CleanupDeadEnemies();
}

void EnemySpawner::Render() {
    // Render all alive enemies
    for (auto& enemy : enemies_) {
        if (enemy && enemy->IsAlive()) {
            enemy->Render();
        }
    }
}

int EnemySpawner::GetAliveEnemyCount() const {
    int count = 0;
    for (const auto& enemy : enemies_) {
        if (enemy && enemy->IsAlive()) {
            count++;
        }
    }
    return count;
}

void EnemySpawner::SpawnEnemy() {
    // Note: WaveManager controls when to spawn, so we don't check spawning_enabled_ here
    
    // Generate spawn position
    glm::vec3 spawn_pos = GenerateSpawnPosition();
    
    // Create new enemy
    auto enemy = std::make_unique<Enemy>();
    if (enemy->Initialize(spawn_pos)) {
        // Get difficulty multiplier from wave manager
        float difficulty_mult = wave_manager_ ? wave_manager_->GetDifficultyMultiplier() : 1.0f;
        
        // 30% chance to be a fast enemy variant
        static std::uniform_real_distribution<float> chance_dist(0.0f, 1.0f);
        float roll = chance_dist(gen_);
        if (roll < 0.3f) {
            enemy->SetSpeed(6.0f * std::min(1.5f, 1.0f + (difficulty_mult - 1.0f) * 0.5f));  // Speed scales slower than HP
            enemy->SetHealth(6.0f * difficulty_mult);    // less HP, but scaled by difficulty
            enemy->SetColor(glm::vec3(1.0f, 1.0f, 0.0f)); // yellow tint
        } else {
            // Apply difficulty multiplier to normal enemies
            float base_health = enemy->GetHealth(); // Get initial health from Enemy::Initialize
            float base_speed = enemy->GetSpeed();
            enemy->SetHealth(base_health * difficulty_mult);
            enemy->SetSpeed(base_speed * std::min(1.5f, 1.0f + (difficulty_mult - 1.0f) * 0.5f));  // Speed scales slower (max 1.5x)
        }
        enemies_.push_back(std::move(enemy));
        std::cout << "Spawned enemy #" << enemies_.size() << " at distance " 
                  << glm::length(spawn_pos) << " from center" << std::endl;
    }
}

void EnemySpawner::CleanupDeadEnemies() {
    // Remove dead enemies
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return !enemy || !enemy->IsAlive();
            }),
        enemies_.end()
    );
}

void EnemySpawner::ClearAllEnemies() {
    enemies_.clear();
}

glm::vec3 EnemySpawner::GenerateSpawnPosition() {
    // Generate random angle around Y axis (0 to 2π)
    float angle = angle_dist_(gen_);
    
    // Generate random height (Z coordinate)
    float height = height_dist_(gen_);
    
    // Calculate X and Y coordinates on the circle
    float x = spawn_radius_ * glm::cos(angle);
    float y = spawn_radius_ * glm::sin(angle);
    
    glm::vec3 spawn_pos(x, y, height);
    
    std::cout << "Generated spawn position: " << spawn_pos.x << ", " 
              << spawn_pos.y << ", " << spawn_pos.z << std::endl;
    
    return spawn_pos;
}

void EnemySpawner::UpdateSpawnTimer(float delta_time) {
    if (!spawning_enabled_) return;
    
    time_since_last_spawn_ += delta_time;
    
    // Check if it's time to spawn
    float spawn_interval = 1.0f / spawn_rate_;
    if (time_since_last_spawn_ >= spawn_interval) {
        SpawnEnemy();
        time_since_last_spawn_ = 0.0f;
    }
}

