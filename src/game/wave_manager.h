#pragma once

#include <vector>
#include <glm/glm.hpp>

class EnemySpawner;
class ItemManager;

class WaveManager {
public:
    WaveManager();
    ~WaveManager() = default;

    void Update(float delta_time);
    void SetEnemySpawner(EnemySpawner* spawner);
    void SetItemManager(ItemManager* item_manager);
    
    // Геттеры для UI
    int GetCurrentWave() const { return current_wave_; }
    int GetEnemiesRemaining() const { return enemies_remaining_; }
    float GetTimeTillNextWave() const { return wave_delay_timer_; }
    bool IsWaveActive() const { return wave_active_; }
    bool IsGameOver() const { return game_over_; }
    int GetTotalScore() const { return total_score_; }
    int GetCoreHealth() const { return core_health_; }
    int GetCurrency() const { return currency_; }
    float GetDifficultyMultiplier() const { return difficulty_multiplier_; }
    
    // Управление игрой
    void StartGame();
    void StartNextWave();
    void OnEnemyDestroyed(const glm::vec3& enemy_position);
    void OnEnemyReachedCore();
    void SetPreparationDuration(float seconds) { wave_delay_duration_ = seconds; }
    void SetInitialPreparation(float seconds) { wave_delay_timer_ = seconds; }
    
    // Обновление только экономики (для паузы)
    void UpdateEconomy();

    // Экономика
    void AddCurrency(int amount) { currency_ += amount; }
    bool SpendCurrency(int amount) { if (currency_ >= amount) { currency_ -= amount; return true; } return false; }

private:
    EnemySpawner* enemy_spawner_;
    ItemManager* item_manager_;
    
    // Состояние волны
    int current_wave_;
    bool wave_active_;
    bool game_over_;
    
    // Враги
    int enemies_remaining_;
    int enemies_spawned_this_wave_;
    int enemies_to_spawn_this_wave_;
    
    // Таймеры
    float wave_delay_timer_;
    float spawn_timer_;
    
    // Конфигурация
    float wave_delay_duration_;      // Задержка между волнами
    float initial_spawn_interval_;   // Начальный интервал спавна
    float spawn_interval_;           // Текущий интервал спавна
    int base_enemies_per_wave_;      // Базовое количество врагов
    
    // Прогрессия
    float difficulty_multiplier_;
    
    // Очки
    int total_score_;
    int core_health_;
    int currency_;
    int reward_per_enemy_;
    int starting_currency_;
    
    void CalculateWaveParameters();
    void SpawnEnemy();
};

