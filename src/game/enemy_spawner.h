#pragma once

#include "enemy.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <random>

class WaveManager;

class EnemySpawner {
public:
    EnemySpawner();
    ~EnemySpawner();

    // Initialize spawner
    bool Initialize();

    // Update spawner logic
    void Update(float delta_time);

    // Render all enemies
    void Render();

    // Spawn controls
    void StartSpawning() { spawning_enabled_ = true; }
    void StopSpawning() { spawning_enabled_ = false; }
    void SetSpawnRate(float rate) { spawn_rate_ = rate; }
    void SetSpawnRadius(float radius) { spawn_radius_ = radius; }

    // Wave manager integration
    void SetWaveManager(WaveManager* wave_manager) { wave_manager_ = wave_manager; }

    // Enemy management
    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }
    int GetEnemyCount() const { return static_cast<int>(enemies_.size()); }
    int GetAliveEnemyCount() const;

    // Spawn a single enemy
    void SpawnEnemy();

    // Clean up dead enemies
    void CleanupDeadEnemies();

private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
    WaveManager* wave_manager_;
    
    // Spawn parameters
    bool spawning_enabled_;
    float spawn_rate_;          // Enemies per second
    float spawn_radius_;        // Distance from center to spawn enemies
    float time_since_last_spawn_;
    
    // Random number generation
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_real_distribution<float> angle_dist_;
    std::uniform_real_distribution<float> height_dist_;
    
    // Generate random spawn position on sphere
    glm::vec3 GenerateSpawnPosition();
    
    // Spawn timer
    void UpdateSpawnTimer(float delta_time);
};

