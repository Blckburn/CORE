// Implementation of game item system
#include "item.h"
#include <random>
#include <iostream>

Item::Item()
    : position_(0.0f)
    , rarity_(ItemRarity::Common)
    , primary_stat_(ItemStat::Damage)
    , secondary_stat_(ItemStat::Damage)
    , primary_bonus_(0.0f)
    , secondary_bonus_(0.0f)
    , legendary_effect_(LegendaryEffect::None)
    , color_(1.0f)
    , active_(false)
    , stack_count_(1) {
}

Item::~Item() {
}

bool Item::Initialize(const glm::vec3& drop_position, ItemRarity rarity) {
    position_ = drop_position;
    rarity_ = rarity;
    active_ = true;
    
    GenerateStats();
    SetColorByRarity();
    
    std::cout << "Item dropped: " << GetName() << " at " 
              << position_.x << ", " << position_.y << ", " << position_.z << std::endl;
    
    return true;
}

void Item::GenerateStats() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> stat_dist(0, 2); // Damage, FireRate, Range
    
    switch (rarity_) {
        case ItemRarity::Common:
            // +10% к одной характеристике
            primary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            primary_bonus_ = 10.0f;
            secondary_bonus_ = 0.0f;
            break;
            
        case ItemRarity::Uncommon:
            // +20% к одной характеристике
            primary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            primary_bonus_ = 20.0f;
            secondary_bonus_ = 0.0f;
            break;
            
        case ItemRarity::Rare:
            // +30% к основной, +10% к второй
            primary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            do {
                secondary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            } while (secondary_stat_ == primary_stat_);
            primary_bonus_ = 30.0f;
            secondary_bonus_ = 10.0f;
            break;
            
        case ItemRarity::Epic:
            // +50% к основной, +30% к второй
            primary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            do {
                secondary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            } while (secondary_stat_ == primary_stat_);
            primary_bonus_ = 50.0f;
            secondary_bonus_ = 30.0f;
            break;
            
        case ItemRarity::Legendary:
            // +100% к основной, +50% к второй + специальный эффект
            primary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            do {
                secondary_stat_ = static_cast<ItemStat>(stat_dist(gen));
            } while (secondary_stat_ == primary_stat_);
            primary_bonus_ = 100.0f;
            secondary_bonus_ = 50.0f;
            
            // Случайный легендарный эффект
            std::uniform_int_distribution<int> effect_dist(1, 5);
            legendary_effect_ = static_cast<LegendaryEffect>(effect_dist(gen));
            break;
    }
}

void Item::SetColorByRarity() {
    switch (rarity_) {
        case ItemRarity::Common:
            color_ = glm::vec3(1.0f, 1.0f, 1.0f); // White
            break;
        case ItemRarity::Uncommon:
            color_ = glm::vec3(0.2f, 0.8f, 0.2f); // Dark Green (more visible on grass)
            break;
        case ItemRarity::Rare:
            color_ = glm::vec3(0.3f, 0.5f, 1.0f); // Blue
            break;
        case ItemRarity::Epic:
            color_ = glm::vec3(0.7f, 0.3f, 1.0f); // Purple
            break;
        case ItemRarity::Legendary:
            color_ = glm::vec3(1.0f, 0.3f, 0.0f); // Red/Orange
            break;
    }
}

std::string Item::GetName() const {
    std::string rarity_name;
    switch (rarity_) {
        case ItemRarity::Common: rarity_name = "Common"; break;
        case ItemRarity::Uncommon: rarity_name = "Uncommon"; break;
        case ItemRarity::Rare: rarity_name = "Rare"; break;
        case ItemRarity::Epic: rarity_name = "Epic"; break;
        case ItemRarity::Legendary: rarity_name = "Legendary"; break;
    }
    
    std::string stat_name;
    switch (primary_stat_) {
        case ItemStat::Damage: stat_name = "Damage"; break;
        case ItemStat::FireRate: stat_name = "FireRate"; break;
        case ItemStat::Range: stat_name = "Range"; break;
        case ItemStat::Special: stat_name = "Special"; break;
    }
    
    return rarity_name + " " + stat_name + " Mod";
}

std::string Item::GetDescription() const {
    std::string desc = "";
    
    // Primary stat
    switch (primary_stat_) {
        case ItemStat::Damage: 
            desc += "+" + std::to_string(static_cast<int>(primary_bonus_)) + "% Damage\n"; 
            break;
        case ItemStat::FireRate: 
            desc += "+" + std::to_string(static_cast<int>(primary_bonus_)) + "% Fire Rate\n"; 
            break;
        case ItemStat::Range: 
            desc += "+" + std::to_string(static_cast<int>(primary_bonus_)) + "% Range\n"; 
            break;
        default: break;
    }
    
    // Secondary stat (if exists)
    if (secondary_bonus_ > 0.0f) {
        switch (secondary_stat_) {
            case ItemStat::Damage: 
                desc += "+" + std::to_string(static_cast<int>(secondary_bonus_)) + "% Damage\n"; 
                break;
            case ItemStat::FireRate: 
                desc += "+" + std::to_string(static_cast<int>(secondary_bonus_)) + "% Fire Rate\n"; 
                break;
            case ItemStat::Range: 
                desc += "+" + std::to_string(static_cast<int>(secondary_bonus_)) + "% Range\n"; 
                break;
            default: break;
        }
    }
    
    // Legendary effect
    if (legendary_effect_ != LegendaryEffect::None) {
        switch (legendary_effect_) {
            case LegendaryEffect::ChainLightning:
                desc += "\n[CHAIN LIGHTNING]\nProjectiles chain to 2 enemies";
                break;
            case LegendaryEffect::SplitShot:
                desc += "\n[SPLIT SHOT]\nProjectiles split on hit";
                break;
            case LegendaryEffect::Multishot:
                desc += "\n[MULTISHOT]\nFires 3 projectiles";
                break;
            case LegendaryEffect::Explosive:
                desc += "\n[EXPLOSIVE]\nAoE damage on hit";
                break;
            case LegendaryEffect::Piercing:
                desc += "\n[PIERCING]\nProjectiles pierce enemies";
                break;
            default: break;
        }
    }
    
    return desc;
}

bool Item::IsSameAs(const Item* other) const {
    if (!other) return false;
    
    // Items are the same if they have identical stats
    return rarity_ == other->rarity_ &&
           primary_stat_ == other->primary_stat_ &&
           secondary_stat_ == other->secondary_stat_ &&
           primary_bonus_ == other->primary_bonus_ &&
           secondary_bonus_ == other->secondary_bonus_ &&
           legendary_effect_ == other->legendary_effect_;
}

