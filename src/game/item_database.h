// Item database for inventory system
#pragma once

#include "item.h"
#include <map>
#include <vector>
#include <string>

// Item template for the database
struct ItemTemplate {
    ItemStat primary_stat;
    ItemStat secondary_stat;
    float primary_bonus;
    float secondary_bonus;
    LegendaryEffect legendary_effect;
    std::string name;
    std::string description;
    bool discovered; // Has this item been found by player?
    int quantity;    // How many of this item the player has
    
    ItemTemplate() : primary_stat(ItemStat::Damage), secondary_stat(ItemStat::Damage), 
                     primary_bonus(0.0f), secondary_bonus(0.0f), legendary_effect(LegendaryEffect::None),
                     name(""), description(""), discovered(false), quantity(0) {}
    
    ItemTemplate(ItemStat primary, ItemStat secondary, float prim_bonus, float sec_bonus, 
                 LegendaryEffect effect, const std::string& item_name, const std::string& desc)
        : primary_stat(primary), secondary_stat(secondary), primary_bonus(prim_bonus), 
          secondary_bonus(sec_bonus), legendary_effect(effect), name(item_name), 
          description(desc), discovered(false), quantity(0) {}
};

// Inventory grid item for UI display
struct InventoryGridItem {
    ItemRarity rarity;
    ItemStat stat_type;
    bool discovered;
    int quantity; // How many of this item the player has
    
    InventoryGridItem() : rarity(ItemRarity::Common), stat_type(ItemStat::Damage), 
                         discovered(false), quantity(0) {}
};

class ItemDatabase {
public:
    ItemDatabase();
    ~ItemDatabase();
    
    // Initialize database with all possible items
    bool Initialize();
    
    // Mark item as discovered when player finds it
    void MarkItemDiscovered(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat, 
                           LegendaryEffect effect = LegendaryEffect::None);
    
    // Add item to inventory (increment quantity)
    void AddItemToInventory(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat,
                           LegendaryEffect effect = LegendaryEffect::None, int quantity = 1);
    
    // Remove item from inventory (decrement quantity)
    void RemoveItemFromInventory(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat,
                                LegendaryEffect effect = LegendaryEffect::None, int quantity = 1);
    
    // Get inventory grid for UI display
    std::vector<std::vector<InventoryGridItem>> GetInventoryGrid() const;
    
    // Get discovered items count
    int GetDiscoveredItemsCount() const;
    
    // Get total possible items count
    int GetTotalItemsCount() const;
    
    // Check if specific item is discovered
    bool IsItemDiscovered(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat,
                         LegendaryEffect effect = LegendaryEffect::None) const;
    
    // Get item template by parameters
    const ItemTemplate* GetItemTemplate(ItemRarity rarity, ItemStat primary_stat, 
                                       ItemStat secondary_stat, LegendaryEffect effect = LegendaryEffect::None) const;
    
    // Get item name by parameters
    std::string GetItemName(ItemRarity rarity, ItemStat primary_stat, 
                           ItemStat secondary_stat, LegendaryEffect effect = LegendaryEffect::None) const;
    
    // Update quantity for discovered item
    void UpdateItemQuantity(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat,
                           LegendaryEffect effect, int quantity);
    
    // Get quantity of specific item type
    int GetItemQuantity(ItemRarity rarity, ItemStat primary_stat, ItemStat secondary_stat,
                       LegendaryEffect effect = LegendaryEffect::None) const;
    
    // Reset all discoveries (for new game)
    void ResetDiscoveries();
    
private:
    // Map of all possible items: rarity -> primary_stat -> secondary_stat -> effect -> template
    std::map<ItemRarity, std::map<ItemStat, std::map<ItemStat, std::map<LegendaryEffect, ItemTemplate>>>> item_templates_;
    
    // Grid data for UI (rarity x stat_type)
    mutable std::vector<std::vector<InventoryGridItem>> inventory_grid_;
    
    // Generate all possible item combinations
    void GenerateItemTemplates();
    
    // Update the inventory grid
    void UpdateInventoryGrid() const;
    
    // Get stat type for grid display (maps multiple stats to single grid position)
    ItemStat GetGridStatType(ItemStat primary_stat, ItemStat secondary_stat) const;
    
    // Helper functions for string conversion
    std::string GetRarityName(ItemRarity rarity) const;
    std::string GetStatName(ItemStat stat) const;
    std::string GetStatDisplayName(ItemStat stat) const;
    std::string GetBonusString(ItemRarity rarity, bool primary) const;
    float GetBonusValue(ItemRarity rarity, bool primary) const;
    std::string GetEffectName(LegendaryEffect effect) const;
    std::string GetEffectDescription(LegendaryEffect effect) const;
};
