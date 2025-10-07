#include "wave_manager.h"
#include "enemy_spawner.h"
#include <iostream>
#include <algorithm>

WaveManager::WaveManager()
    : enemy_spawner_(nullptr)
    , current_wave_(0)
    , wave_active_(false)
    , game_over_(false)
    , enemies_remaining_(0)
    , enemies_spawned_this_wave_(0)
    , enemies_to_spawn_this_wave_(0)
    , wave_delay_timer_(0.0f)
    , spawn_timer_(0.0f)
    , wave_delay_duration_(10.0f)     // 10 секунд между волнами для подготовки
    , initial_spawn_interval_(0.5f)   // 2 врага в секунду изначально
    , spawn_interval_(0.5f)
    , base_enemies_per_wave_(10)      // 10 врагов в первой волне
    , difficulty_multiplier_(1.0f)
    , total_score_(0)
    , core_health_(10)                // Снижено в 10 раз
    , currency_(0)
    , reward_per_enemy_(1)            // Снижено в 10 раз
    , starting_currency_(6) {         // Снижено в 10 раз
}

void WaveManager::SetEnemySpawner(EnemySpawner* spawner) {
    enemy_spawner_ = spawner;
}

void WaveManager::StartGame() {
    current_wave_ = 0;
    wave_active_ = false;
    game_over_ = false;
    enemies_remaining_ = 0;
    total_score_ = 0;
    core_health_ = 10; // Снижено в 10 раз
    currency_ = starting_currency_;
    wave_delay_timer_ = 10.0f; // Увеличили задержку перед первой волной для подготовки
    
    std::cout << "=== GAME STARTED ===" << std::endl;
    std::cout << "Get ready for wave 1..." << std::endl;
    std::cout << "Wave delay: " << wave_delay_timer_ << " seconds" << std::endl;
    std::cout.flush();
}

void WaveManager::StartNextWave() {
    if (game_over_) return;
    
    current_wave_++;
    wave_active_ = true;
    enemies_spawned_this_wave_ = 0;
    spawn_timer_ = 0.0f;
    
    CalculateWaveParameters();
    
    enemies_remaining_ = enemies_to_spawn_this_wave_;
    
    std::cout << "\n=== WAVE " << current_wave_ << " STARTED ===" << std::endl;
    std::cout << "Enemies: " << enemies_to_spawn_this_wave_ << std::endl;
    std::cout << "Spawn interval: " << spawn_interval_ << "s" << std::endl;
    std::cout.flush();
}

void WaveManager::CalculateWaveParameters() {
    // Увеличение количества врагов: 10, 15, 21, 28, 36, 45...
    enemies_to_spawn_this_wave_ = base_enemies_per_wave_ + (current_wave_ - 1) * 5 + (current_wave_ - 1) * (current_wave_ - 1) / 2;
    
    // Уменьшение интервала спавна (но не меньше 0.15 секунды)
    spawn_interval_ = std::max(0.15f, initial_spawn_interval_ - (current_wave_ - 1) * 0.03f);
    
    // Увеличение сложности (скорость и здоровье врагов будем использовать позже)
    difficulty_multiplier_ = 1.0f + (current_wave_ - 1) * 0.15f;
}

void WaveManager::SpawnEnemy() {
    if (!enemy_spawner_) {
        std::cout << "ERROR: No enemy spawner set!" << std::endl;
        std::cout.flush();
        return;
    }
    
    enemy_spawner_->SpawnEnemy();
    enemies_spawned_this_wave_++;
}

void WaveManager::OnEnemyDestroyed() {
    if (enemies_remaining_ > 0) {
        enemies_remaining_--;
        total_score_ += 1; // очки - снижено в 10 раз
        // Немного большая награда за быстрых врагов (цвет желтый)
        int reward = reward_per_enemy_;
        // Простой способ: если у врага было меньше базового HP при смерти, добавим 20%
        // (в дальнейшем можно хранить тип врага явно)
        reward = reward_per_enemy_;
        currency_ += reward; // валюта
        
        std::cout << "Enemy destroyed! Remaining: " << enemies_remaining_ << " | Score: " << total_score_ << std::endl;
        
        // Проверяем, завершена ли волна
        if (enemies_remaining_ == 0 && enemies_spawned_this_wave_ >= enemies_to_spawn_this_wave_) {
            wave_active_ = false;
            wave_delay_timer_ = wave_delay_duration_;
            
            std::cout << "\n=== WAVE " << current_wave_ << " COMPLETED ===" << std::endl;
            std::cout << "Score: " << total_score_ << std::endl;
            std::cout << "Next wave in " << wave_delay_duration_ << " seconds..." << std::endl;
        }
    }
}

void WaveManager::OnEnemyReachedCore() {
    core_health_ -= 1; // Снижено в 10 раз
    
    if (enemies_remaining_ > 0) {
        enemies_remaining_--;
    }
    
    std::cout << "CORE HIT! Health: " << core_health_ << " | Remaining enemies: " << enemies_remaining_ << std::endl;
    
    if (core_health_ <= 0) {
        game_over_ = true;
        wave_active_ = false;
        std::cout << "\n=== GAME OVER ===" << std::endl;
        std::cout << "Final Score: " << total_score_ << std::endl;
        std::cout << "Waves Survived: " << current_wave_ << std::endl;
    } else if (enemies_remaining_ == 0 && enemies_spawned_this_wave_ >= enemies_to_spawn_this_wave_) {
        wave_active_ = false;
        wave_delay_timer_ = wave_delay_duration_;
        
        std::cout << "\n=== WAVE " << current_wave_ << " COMPLETED ===" << std::endl;
        std::cout << "Score: " << total_score_ << std::endl;
        std::cout << "Core Health: " << core_health_ << std::endl;
        std::cout << "Next wave in " << wave_delay_duration_ << " seconds..." << std::endl;
    }
}

void WaveManager::Update(float delta_time) {
    if (game_over_) return;
    
    // Если волна не активна, отсчитываем время до следующей
    if (!wave_active_) {
        if (wave_delay_timer_ > 0.0f) {
            wave_delay_timer_ -= delta_time;
            
            if (wave_delay_timer_ <= 0.0f) {
                StartNextWave();
            }
        }
    } else {
        // Во время активной волны спавним врагов по таймеру
        spawn_timer_ += delta_time;
        
        if (spawn_timer_ >= spawn_interval_ && enemies_spawned_this_wave_ < enemies_to_spawn_this_wave_) {
            SpawnEnemy();
            spawn_timer_ = 0.0f;
        }
    }
}

