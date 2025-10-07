#pragma once

#include "item.h"
#include <vector>
#include <memory>
#include <random>
#include <glm/glm.hpp>

class ItemManager {
public:
    ItemManager();
    ~ItemManager();
    
    // Initialize
    bool Initialize();
    
    // Drop item at position with random rarity
    void DropItem(const glm::vec3& position);
    
    // Pickup item at position (returns true if item was picked up)
    Item* PickupItemAtPosition(const glm::vec3& position, float radius = 2.0f);
    
    // Get item at position for hover detection
    Item* GetItemAtPosition(const glm::vec3& position, float radius = 2.0f);
    
    // Getters
    const std::vector<std::unique_ptr<Item>>& GetDroppedItems() const { return dropped_items_; }
    const std::vector<std::unique_ptr<Item>>& GetInventory() const { return inventory_; }
    int GetInventoryCount() const { return static_cast<int>(inventory_.size()); }
    
    // Clean up picked items
    void CleanupPickedItems();
    
    // Clear all items
    void ClearAll();
    
private:
    std::vector<std::unique_ptr<Item>> dropped_items_; // Items in the world
    std::vector<std::unique_ptr<Item>> inventory_;     // Items in player inventory
    
    // Random number generation for rarity
    std::random_device rd_;
    std::mt19937 gen_;
    
    // Generate random rarity based on drop chances
    ItemRarity GenerateRandomRarity();
};

