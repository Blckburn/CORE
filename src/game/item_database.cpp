// Implementation of item database system
#include "item_database.h"
#include <iostream>
#include <algorithm>

ItemDatabase::ItemDatabase() {
    inventory_grid_.resize(5, std::vector<InventoryGridItem>(4)); // 5 rarities x 4 stat types
}

ItemDatabase::~ItemDatabase() {
}

bool ItemDatabase::Initialize() {
    std::cout << "Initializing item database..." << std::endl;
    
    GenerateItemTemplates();
    UpdateInventoryGrid();
    
    std::cout << "Item database initialized with " << GetTotalItemsCount() << " possible items" << std::endl;
    return true;
}

void ItemDatabase::GenerateItemTemplates() {
    // Clear existing templates
    item_templates_.clear();
    
    // Stat combinations for different rarities
    std::vector<ItemStat> stats = {ItemStat::Damage, ItemStat::FireRate, ItemStat::Range};
    std::vector<ItemRarity> rarities = {ItemRarity::Common, ItemRarity::Uncommon, ItemRarity::Rare, ItemRarity::Epic, ItemRarity::Legendary};
    
    // Generate items for each rarity
    for (auto rarity : rarities) {
        for (auto primary_stat : stats) {
            if (rarity == ItemRarity::Common || rarity == ItemRarity::Uncommon) {
                // Common/Uncommon: only primary stat
                std::string name = GetRarityName(rarity) + " " + GetStatName(primary_stat) + " Mod";
                std::string desc = "+" + GetBonusString(rarity, true) + "% " + GetStatDisplayName(primary_stat);
                
                item_templates_[rarity][primary_stat][ItemStat::Damage][LegendaryEffect::None] = 
                    ItemTemplate(primary_stat, ItemStat::Damage, GetBonusValue(rarity, true), 0.0f, 
                               LegendaryEffect::None, name, desc);
            } else {
                // Rare/Epic/Legendary: primary + secondary stats
                for (auto secondary_stat : stats) {
                    if (secondary_stat != primary_stat) {
                        std::string name = GetRarityName(rarity) + " " + GetStatName(primary_stat) + 
                                         "/" + GetStatName(secondary_stat) + " Mod";
                        std::string desc = "+" + GetBonusString(rarity, true) + "% " + GetStatDisplayName(primary_stat) + 
                                         "\n+" + GetBonusString(rarity, false) + "% " + GetStatDisplayName(secondary_stat);
                        
                        LegendaryEffect effect = LegendaryEffect::None;
                        if (rarity == ItemRarity::Legendary) {
                            // Generate legendary items with different effects
                            for (int effect_val = 1; effect_val <= 5; ++effect_val) {
                                effect = static_cast<LegendaryEffect>(effect_val);
                                std::string legendary_name = GetRarityName(rarity) + " " + GetStatName(primary_stat) + 
                                                            "/" + GetStatName(secondary_stat) + " " + GetEffectName(effect);
                                std::string legendary_desc = desc + "\n[" + GetEffectName(effect) + "]\n" + GetEffectDescription(effect);
                                
                                item_templates_[rarity][primary_stat][secondary_stat][effect] = 
                                    ItemTemplate(primary_stat, secondary_stat, GetBonusValue(rarity, true), 
                                               GetBonusValue(rarity, false), effect, legendary_name, legendary_desc);
                            }
                        } else {
                            item_templates_[rarity][primary_stat][secondary_stat][LegendaryEffect::None] = 
                                ItemTemplate(primary_stat, secondary_stat, GetBonusValue(rarity, true), 
                                           GetBonusValue(rarity, false), LegendaryEffect::None, name, desc);
                        }
                    }
                }
            }
        }
    }
}

void ItemDatabase::MarkItemDiscovered(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect) {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    effect_it->second.discovered = true;
                    UpdateInventoryGrid();
                    std::cout << "Item discovered: " << effect_it->second.name << std::endl;
                }
            }
        }
    }
}

void ItemDatabase::AddItemToInventory(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect, int quantity) {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    effect_it->second.discovered = true;
                    effect_it->second.quantity += quantity;
                    UpdateInventoryGrid();
                    std::cout << "Added " << quantity << " " << effect_it->second.name << " (total: " << effect_it->second.quantity << ")" << std::endl;
                }
            }
        }
    }
}

void ItemDatabase::RemoveItemFromInventory(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect, int quantity) {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    effect_it->second.quantity = std::max(0, effect_it->second.quantity - quantity);
                    if (effect_it->second.quantity == 0) {
                        effect_it->second.discovered = false;
                    }
                    UpdateInventoryGrid();
                    std::cout << "Removed " << quantity << " " << effect_it->second.name << " (remaining: " << effect_it->second.quantity << ")" << std::endl;
                }
            }
        }
    }
}

std::vector<std::vector<InventoryGridItem>> ItemDatabase::GetInventoryGrid() const {
    UpdateInventoryGrid();
    return inventory_grid_;
}

int ItemDatabase::GetDiscoveredItemsCount() const {
    int count = 0;
    for (const auto& rarity_pair : item_templates_) {
        for (const auto& primary_pair : rarity_pair.second) {
            for (const auto& secondary_pair : primary_pair.second) {
                for (const auto& effect_pair : secondary_pair.second) {
                    if (effect_pair.second.discovered) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

int ItemDatabase::GetTotalItemsCount() const {
    int count = 0;
    for (const auto& rarity_pair : item_templates_) {
        for (const auto& primary_pair : rarity_pair.second) {
            for (const auto& secondary_pair : primary_pair.second) {
                count += secondary_pair.second.size();
            }
        }
    }
    return count;
}

bool ItemDatabase::IsItemDiscovered(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect) const {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    return effect_it->second.discovered;
                }
            }
        }
    }
    return false;
}

const ItemTemplate* ItemDatabase::GetItemTemplate(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect) const {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    return &effect_it->second;
                }
            }
        }
    }
    return nullptr;
}

std::string ItemDatabase::GetItemName(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect) const {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    return effect_it->second.name;
                }
            }
        }
    }
    return "Unknown Item";
}

void ItemDatabase::UpdateItemQuantity(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect, int quantity) {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    effect_it->second.quantity = quantity;
                    if (quantity > 0) {
                        effect_it->second.discovered = true;
                    } else {
                        effect_it->second.discovered = false;
                    }
                    UpdateInventoryGrid();
                }
            }
        }
    }
}

int ItemDatabase::GetItemQuantity(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, LegendaryEffect effect) const {
    auto rarity_it = item_templates_.find(rarity);
    if (rarity_it != item_templates_.end()) {
        auto primary_it = rarity_it->second.find(primary_stat);
        if (primary_it != rarity_it->second.end()) {
            auto secondary_it = primary_it->second.find(secondary_stat);
            if (secondary_it != primary_it->second.end()) {
                auto effect_it = secondary_it->second.find(effect);
                if (effect_it != secondary_it->second.end()) {
                    return effect_it->second.quantity;
                }
            }
        }
    }
    return 0;
}

void ItemDatabase::ResetDiscoveries() {
    for (auto& rarity_pair : item_templates_) {
        for (auto& primary_pair : rarity_pair.second) {
            for (auto& secondary_pair : primary_pair.second) {
                for (auto& effect_pair : secondary_pair.second) {
                    effect_pair.second.discovered = false;
                }
            }
        }
    }
    UpdateInventoryGrid();
}

void ItemDatabase::UpdateInventoryGrid() const {
    // Reset grid
    for (auto& row : inventory_grid_) {
        for (auto& cell : row) {
            cell.discovered = false;
            cell.quantity = 0;
        }
    }
    
    // Update grid based on discovered items
    for (const auto& rarity_pair : item_templates_) {
        int rarity_index = static_cast<int>(rarity_pair.first);
        
        for (const auto& primary_pair : rarity_pair.second) {
            for (const auto& secondary_pair : primary_pair.second) {
                for (const auto& effect_pair : secondary_pair.second) {
                    if (effect_pair.second.discovered && effect_pair.second.quantity > 0) {
                        ItemStat grid_stat = GetGridStatType(primary_pair.first, secondary_pair.first);
                        int stat_index = static_cast<int>(grid_stat);
                        
                        if (rarity_index < inventory_grid_.size() && stat_index < inventory_grid_[rarity_index].size()) {
                            inventory_grid_[rarity_index][stat_index].discovered = true;
                            inventory_grid_[rarity_index][stat_index].quantity += effect_pair.second.quantity;
                        }
                    }
                }
            }
        }
    }
}

ItemStat ItemDatabase::GetGridStatType(ItemStat primary_stat, ItemStat secondary_stat) const {
    // Map to grid positions: Damage=0, FireRate=1, Range=2, Special=3
    if (primary_stat == ItemStat::Damage || secondary_stat == ItemStat::Damage) {
        return ItemStat::Damage;
    } else if (primary_stat == ItemStat::FireRate || secondary_stat == ItemStat::FireRate) {
        return ItemStat::FireRate;
    } else if (primary_stat == ItemStat::Range || secondary_stat == ItemStat::Range) {
        return ItemStat::Range;
    }
    return ItemStat::Special;
}

// Helper functions for string conversion
std::string ItemDatabase::GetRarityName(ItemRarity rarity) const {
    switch (rarity) {
        case ItemRarity::Common: return "Common";
        case ItemRarity::Uncommon: return "Uncommon";
        case ItemRarity::Rare: return "Rare";
        case ItemRarity::Epic: return "Epic";
        case ItemRarity::Legendary: return "Legendary";
        default: return "Unknown";
    }
}

std::string ItemDatabase::GetStatName(ItemStat stat) const {
    switch (stat) {
        case ItemStat::Damage: return "Damage";
        case ItemStat::FireRate: return "FireRate";
        case ItemStat::Range: return "Range";
        case ItemStat::Special: return "Special";
        default: return "Unknown";
    }
}

std::string ItemDatabase::GetStatDisplayName(ItemStat stat) const {
    switch (stat) {
        case ItemStat::Damage: return "Damage";
        case ItemStat::FireRate: return "Fire Rate";
        case ItemStat::Range: return "Range";
        case ItemStat::Special: return "Special";
        default: return "Unknown";
    }
}

std::string ItemDatabase::GetBonusString(ItemRarity rarity, bool primary) const {
    float bonus = GetBonusValue(rarity, primary);
    return std::to_string(static_cast<int>(bonus));
}

float ItemDatabase::GetBonusValue(ItemRarity rarity, bool primary) const {
    switch (rarity) {
        case ItemRarity::Common: return primary ? 10.0f : 0.0f;
        case ItemRarity::Uncommon: return primary ? 20.0f : 0.0f;
        case ItemRarity::Rare: return primary ? 30.0f : 10.0f;
        case ItemRarity::Epic: return primary ? 50.0f : 30.0f;
        case ItemRarity::Legendary: return primary ? 100.0f : 50.0f;
        default: return 0.0f;
    }
}

std::string ItemDatabase::GetEffectName(LegendaryEffect effect) const {
    switch (effect) {
        case LegendaryEffect::ChainLightning: return "Chain Lightning";
        case LegendaryEffect::SplitShot: return "Split Shot";
        case LegendaryEffect::Multishot: return "Multishot";
        case LegendaryEffect::Explosive: return "Explosive";
        case LegendaryEffect::Piercing: return "Piercing";
        default: return "None";
    }
}

std::string ItemDatabase::GetEffectDescription(LegendaryEffect effect) const {
    switch (effect) {
        case LegendaryEffect::ChainLightning: return "Projectiles chain to 2 enemies";
        case LegendaryEffect::SplitShot: return "Projectiles split on hit";
        case LegendaryEffect::Multishot: return "Fires 3 projectiles";
        case LegendaryEffect::Explosive: return "AoE damage on hit";
        case LegendaryEffect::Piercing: return "Projectiles pierce enemies";
        default: return "No special effect";
    }
}
