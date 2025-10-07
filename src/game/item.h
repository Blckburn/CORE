#pragma once

#include <glm/glm.hpp>
#include <string>

// Item rarity levels
enum class ItemRarity {
    Common,      // White - базовый бонус
    Uncommon,    // Green - улучшенный бонус
    Rare,        // Blue - два бонуса
    Epic,        // Purple - большие бонусы
    Legendary    // Red - уникальные эффекты
};

// Item stat types
enum class ItemStat {
    Damage,      // Увеличение урона
    FireRate,    // Увеличение скорострельности
    Range,       // Увеличение дальности
    Special      // Специальный эффект (только для легендарных)
};

// Legendary special effects
enum class LegendaryEffect {
    None,
    ChainLightning,  // Снаряд отскакивает на 2 дополнительных врагов
    SplitShot,       // Снаряд раздваивается при попадании
    Multishot,       // Стреляет 3 снарядами одновременно
    Explosive,       // AoE урон вокруг попадания
    Piercing         // Снаряд пробивает врагов насквозь
};

class Item {
public:
    Item();
    ~Item();
    
    // Initialize item with parameters
    bool Initialize(const glm::vec3& drop_position, ItemRarity rarity);
    
    // Getters
    glm::vec3 GetPosition() const { return position_; }
    ItemRarity GetRarity() const { return rarity_; }
    ItemStat GetPrimaryStat() const { return primary_stat_; }
    ItemStat GetSecondaryStat() const { return secondary_stat_; }
    float GetPrimaryBonus() const { return primary_bonus_; }
    float GetSecondaryBonus() const { return secondary_bonus_; }
    LegendaryEffect GetEffect() const { return legendary_effect_; }
    glm::vec3 GetColor() const { return color_; }
    bool IsActive() const { return active_; }
    std::string GetName() const;
    std::string GetDescription() const;
    
    // Setters
    void SetActive(bool active) { active_ = active; }
    void Pickup() { active_ = false; } // Деактивировать при подборе
    
private:
    glm::vec3 position_;          // Позиция в мире (где выпал)
    ItemRarity rarity_;           // Редкость предмета
    ItemStat primary_stat_;       // Основная характеристика
    ItemStat secondary_stat_;     // Вторичная характеристика (для редких+)
    float primary_bonus_;         // Значение основного бонуса (в процентах)
    float secondary_bonus_;       // Значение вторичного бонуса
    LegendaryEffect legendary_effect_; // Спец эффект для легендарок
    glm::vec3 color_;             // Цвет предмета по редкости
    bool active_;                 // Активен ли предмет в мире
    
    // Generate random stats based on rarity
    void GenerateStats();
    void SetColorByRarity();
};

