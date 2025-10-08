// Inventory UI system for displaying discovered items
#pragma once

#include "item_database.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Renderer;

class InventoryUI {
public:
    InventoryUI();
    ~InventoryUI();
    
    // Initialize UI
    bool Initialize(Renderer* renderer);
    
    // Update and render inventory grid
    void Render(float delta_time);
    
    // Set visibility
    void SetVisible(bool visible) { visible_ = visible; }
    bool IsVisible() const { return visible_; }
    
    // Update inventory data
    void UpdateInventoryData(const ItemDatabase* item_database);
    
    // Get grid position for item tooltip
    bool GetItemAtPosition(const glm::vec2& screen_pos, ItemRarity& rarity, ItemStat& stat_type, bool& discovered);
    
    // Toggle visibility
    void Toggle() { visible_ = !visible_; }
    
private:
    Renderer* renderer_;
    bool visible_;
    bool initialized_;
    
    // Grid data
    std::vector<std::vector<InventoryGridItem>> inventory_grid_;
    
    // UI constants
    static const float GRID_START_X;
    static const float GRID_START_Y;
    static const float CELL_SIZE;
    static const float CELL_SPACING;
    static const int GRID_ROWS;    // Number of stat types
    static const int GRID_COLS;    // Number of rarity types
    
    // Colors for different rarities
    static const glm::vec3 COMMON_COLOR;
    static const glm::vec3 UNCOMMON_COLOR;
    static const glm::vec3 RARE_COLOR;
    static const glm::vec3 EPIC_COLOR;
    static const glm::vec3 LEGENDARY_COLOR;
    static const glm::vec3 UNDISCOVERED_COLOR;
    static const glm::vec3 TEXT_COLOR;
    
    // Render methods
    void RenderGrid();
    void RenderCell(int row, int col, const InventoryGridItem& item);
    void RenderHeaders();
    void RenderLegend();
    
    // Helper methods
    glm::vec3 GetRarityColor(ItemRarity rarity) const;
    std::string GetRarityName(ItemRarity rarity) const;
    std::string GetStatName(ItemStat stat) const;
    glm::vec2 GetCellPosition(int row, int col) const;
    glm::vec2 GetCellSize() const;
    
    // Check if point is inside cell
    bool IsPointInCell(const glm::vec2& point, int row, int col) const;
};

