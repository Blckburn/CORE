#include "item_manager.h"
#include <iostream>
#include <algorithm>

ItemManager::ItemManager()
    : rd_()
    , gen_(rd_()) {
}

ItemManager::~ItemManager() {
    ClearAll();
}

bool ItemManager::Initialize() {
    std::cout << "Initializing item manager..." << std::endl;
    dropped_items_.clear();
    inventory_.clear();
    return true;
}

void ItemManager::DropItem(const glm::vec3& position) {
    ItemRarity rarity = GenerateRandomRarity();
    
    auto item = std::make_unique<Item>();
    if (item->Initialize(position, rarity)) {
        dropped_items_.push_back(std::move(item));
    }
}

Item* ItemManager::PickupItemAtPosition(const glm::vec3& position, float radius) {
    for (auto& item : dropped_items_) {
        if (!item || !item->IsActive()) continue;
        
        float distance = glm::length(item->GetPosition() - position);
        if (distance <= radius) {
            // Move item to inventory
            item->Pickup();
            inventory_.push_back(std::move(item));
            
            std::cout << "Item picked up! Inventory: " << inventory_.size() << std::endl;
            
            // Return pointer to item in inventory
            return inventory_.back().get();
        }
    }
    return nullptr;
}

Item* ItemManager::GetItemAtPosition(const glm::vec3& position, float radius) {
    for (auto& item : dropped_items_) {
        if (!item || !item->IsActive()) continue;
        
        float distance = glm::length(item->GetPosition() - position);
        if (distance <= radius) {
            return item.get();
        }
    }
    return nullptr;
}

void ItemManager::RemoveFromInventory(int index) {
    if (index >= 0 && index < static_cast<int>(inventory_.size())) {
        inventory_.erase(inventory_.begin() + index);
        std::cout << "Removed item from inventory index " << index << std::endl;
    }
}

void ItemManager::CleanupPickedItems() {
    // Remove picked up items from dropped list
    dropped_items_.erase(
        std::remove_if(dropped_items_.begin(), dropped_items_.end(),
            [](const std::unique_ptr<Item>& item) {
                return !item || !item->IsActive();
            }),
        dropped_items_.end()
    );
}

void ItemManager::ClearAll() {
    dropped_items_.clear();
    inventory_.clear();
}

ItemRarity ItemManager::GenerateRandomRarity() {
    // Drop chances:
    // Common: 50%
    // Uncommon: 30%
    // Rare: 15%
    // Epic: 4%
    // Legendary: 1%
    
    std::uniform_real_distribution<float> dist(0.0f, 100.0f);
    float roll = dist(gen_);
    
    if (roll < 1.0f) {
        return ItemRarity::Legendary;
    } else if (roll < 5.0f) {
        return ItemRarity::Epic;
    } else if (roll < 20.0f) {
        return ItemRarity::Rare;
    } else if (roll < 50.0f) {
        return ItemRarity::Uncommon;
    } else {
        return ItemRarity::Common;
    }
}

