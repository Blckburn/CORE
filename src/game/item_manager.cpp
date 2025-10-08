// Implementation of item collection management
#include "item_manager.h"
#include <iostream>
#include <algorithm>

ItemManager::ItemManager()
    : rd_()
    , gen_(rd_())
    , item_database_(std::make_unique<ItemDatabase>()) {
}

ItemManager::~ItemManager() {
    ClearAll();
}

bool ItemManager::Initialize() {
    std::cout << "Initializing item manager..." << std::endl;
    dropped_items_.clear();
    inventory_.clear();
    
    // Initialize item database
    if (!item_database_->Initialize()) {
        std::cerr << "Failed to initialize item database!" << std::endl;
        return false;
    }
    
    return true;
}

void ItemManager::DropItem(const glm::vec3& position) {
    ItemRarity rarity = GenerateRandomRarity();
    
    auto item = std::make_unique<Item>();
    if (item->Initialize(position, rarity)) {
        dropped_items_.push_back(std::move(item));
        
        // Auto-cleanup if too many items on ground
        CleanupOldDrops(MAX_DROPPED_ITEMS);
    }
}

Item* ItemManager::PickupItemAtPosition(const glm::vec3& position, float radius) {
    for (auto& item : dropped_items_) {
        if (!item || !item->IsActive()) continue;
        
        float distance = glm::length(item->GetPosition() - position);
        if (distance <= radius) {
            item->Pickup();
            
            // Check if we already have the same item in inventory (for stacking)
            Item* found_stack = nullptr;
            for (auto& inv_item : inventory_) {
                if (inv_item && inv_item->IsSameAs(item.get())) {
                    inv_item->AddToStack(1);
                    found_stack = inv_item.get();
                    std::cout << "Item stacked! Stack count: " << inv_item->GetStackCount() << std::endl;
                    break;
                }
            }
            
            // If not found in inventory, add as new item
            if (!found_stack) {
                inventory_.push_back(std::move(item));
                found_stack = inventory_.back().get();
                std::cout << "Item picked up! Inventory: " << inventory_.size() << std::endl;
            }
            
            // Add item to inventory database
            if (item_database_ && found_stack) {
                item_database_->AddItemToInventory(found_stack->GetRarity(), 
                                                 found_stack->GetPrimaryStat(), 
                                                 found_stack->GetSecondaryStat(),
                                                 found_stack->GetEffect(),
                                                 1); // Add 1 item
            }
            
            return found_stack;
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
        auto& item = inventory_[index];
        if (item->GetStackCount() > 1) {
            // Reduce stack count
            item->RemoveFromStack(1);
            
            // Update database
            if (item_database_) {
                item_database_->RemoveItemFromInventory(item->GetRarity(),
                                                      item->GetPrimaryStat(),
                                                      item->GetSecondaryStat(),
                                                      item->GetEffect(),
                                                      1);
            }
            
            std::cout << "Removed 1 from stack, remaining: " << item->GetStackCount() << std::endl;
        } else {
            // Remove item completely if stack is 1
            if (item_database_) {
                item_database_->RemoveItemFromInventory(item->GetRarity(),
                                                      item->GetPrimaryStat(),
                                                      item->GetSecondaryStat(),
                                                      item->GetEffect(),
                                                      1);
            }
            
            inventory_.erase(inventory_.begin() + index);
            std::cout << "Removed item from inventory index " << index << std::endl;
        }
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

void ItemManager::CleanupOldDrops(int max_drops) {
    // Remove oldest items if too many on ground
    while (static_cast<int>(dropped_items_.size()) > max_drops) {
        // Find and remove first active item (oldest)
        for (auto it = dropped_items_.begin(); it != dropped_items_.end(); ++it) {
            if (*it && (*it)->IsActive()) {
                std::cout << "Auto-removing old dropped item (too many on ground)" << std::endl;
                dropped_items_.erase(it);
                break;
            }
        }
        break; // Remove one at a time
    }
}

