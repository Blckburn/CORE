// Implementation of inventory UI system
#include "inventory_ui.h"
#include "graphics/renderer.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// UI constants
const float InventoryUI::GRID_START_X = 50.0f;
const float InventoryUI::GRID_START_Y = 100.0f;
const float InventoryUI::CELL_SIZE = 80.0f;
const float InventoryUI::CELL_SPACING = 10.0f;
const int InventoryUI::GRID_ROWS = 4;  // Damage, FireRate, Range, Special
const int InventoryUI::GRID_COLS = 5;  // Common, Uncommon, Rare, Epic, Legendary

// Colors
const glm::vec3 InventoryUI::COMMON_COLOR(1.0f, 1.0f, 1.0f);        // White
const glm::vec3 InventoryUI::UNCOMMON_COLOR(0.2f, 0.8f, 0.2f);      // Green
const glm::vec3 InventoryUI::RARE_COLOR(0.3f, 0.5f, 1.0f);          // Blue
const glm::vec3 InventoryUI::EPIC_COLOR(0.7f, 0.3f, 1.0f);          // Purple
const glm::vec3 InventoryUI::LEGENDARY_COLOR(1.0f, 0.3f, 0.0f);     // Red/Orange
const glm::vec3 InventoryUI::UNDISCOVERED_COLOR(0.3f, 0.3f, 0.3f);  // Dark gray
const glm::vec3 InventoryUI::TEXT_COLOR(1.0f, 1.0f, 1.0f);          // White

InventoryUI::InventoryUI()
    : renderer_(nullptr)
    , visible_(false)
    , initialized_(false) {
}

InventoryUI::~InventoryUI() {
}

bool InventoryUI::Initialize(Renderer* renderer) {
    if (!renderer) {
        std::cerr << "InventoryUI: Invalid renderer pointer!" << std::endl;
        return false;
    }
    
    renderer_ = renderer;
    initialized_ = true;
    
    std::cout << "InventoryUI initialized successfully" << std::endl;
    return true;
}

void InventoryUI::Render(float delta_time) {
    if (!initialized_ || !renderer_ || !visible_) {
        return;
    }
    
    RenderHeaders();
    RenderGrid();
    RenderLegend();
}

void InventoryUI::UpdateInventoryData(const ItemDatabase* item_database) {
    if (item_database) {
        inventory_grid_ = item_database->GetInventoryGrid();
    }
}

bool InventoryUI::GetItemAtPosition(const glm::vec2& screen_pos, ItemRarity& rarity, ItemStat& stat_type, bool& discovered) {
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            if (IsPointInCell(screen_pos, row, col)) {
                if (row < static_cast<int>(inventory_grid_.size()) && 
                    col < static_cast<int>(inventory_grid_[row].size())) {
                    const auto& item = inventory_grid_[row][col];
                    rarity = item.rarity;
                    stat_type = item.stat_type;
                    discovered = item.discovered;
                    return true;
                }
            }
        }
    }
    return false;
}

void InventoryUI::RenderHeaders() {
    // Render column headers (rarities)
    std::vector<std::string> rarity_names = {
        "Common", "Uncommon", "Rare", "Epic", "Legendary"
    };
    
    for (int col = 0; col < GRID_COLS; ++col) {
        glm::vec2 pos = GetCellPosition(-1, col);
        pos.y -= 30.0f; // Above the grid
        
        renderer_->RenderText(rarity_names[col], pos.x, pos.y, 0.8f, TEXT_COLOR);
    }
    
    // Render row headers (stat types)
    std::vector<std::string> stat_names = {
        "Damage", "Fire Rate", "Range", "Special"
    };
    
    for (int row = 0; row < GRID_ROWS; ++row) {
        glm::vec2 pos = GetCellPosition(row, -1);
        pos.x -= 80.0f; // Left of the grid
        
        renderer_->RenderText(stat_names[row], pos.x, pos.y, 0.8f, TEXT_COLOR);
    }
}

void InventoryUI::RenderGrid() {
    for (int row = 0; row < GRID_ROWS; ++row) {
        for (int col = 0; col < GRID_COLS; ++col) {
            InventoryGridItem item;
            if (row < static_cast<int>(inventory_grid_.size()) && 
                col < static_cast<int>(inventory_grid_[row].size())) {
                item = inventory_grid_[row][col];
            }
            
            RenderCell(row, col, item);
        }
    }
}

void InventoryUI::RenderCell(int row, int col, const InventoryGridItem& item) {
    glm::vec2 pos = GetCellPosition(row, col);
    glm::vec2 size = GetCellSize();
    
    // Determine color based on discovery status and rarity
    glm::vec3 color;
    if (item.discovered) {
        color = GetRarityColor(item.rarity);
    } else {
        color = UNDISCOVERED_COLOR;
    }
    
    // Render cell background
    renderer_->RenderQuad(pos.x, pos.y, size.x, size.y, color);
    
    // Render cell border
    renderer_->RenderQuadOutline(pos.x, pos.y, size.x, size.y, glm::vec3(0.5f, 0.5f, 0.5f), 2.0f);
    
    // Render quantity if discovered and > 0
    if (item.discovered && item.quantity > 0) {
        std::stringstream ss;
        ss << item.quantity;
        renderer_->RenderText(ss.str(), pos.x + 5.0f, pos.y + 5.0f, 0.8f, TEXT_COLOR);
    }
    
    // Render discovery indicator
    if (item.discovered) {
        renderer_->RenderText("+", pos.x + size.x - 15.0f, pos.y + size.y - 20.0f, 0.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    } else {
        renderer_->RenderText("?", pos.x + size.x/2 - 5.0f, pos.y + size.y/2 - 5.0f, 1.0f, glm::vec3(0.5f, 0.5f, 0.5f));
    }
}

void InventoryUI::RenderLegend() {
    float legend_x = GRID_START_X + (GRID_COLS * (CELL_SIZE + CELL_SPACING)) + 50.0f;
    float legend_y = GRID_START_Y;
    
    renderer_->RenderText("Legend:", legend_x, legend_y, 1.0f, TEXT_COLOR);
    legend_y += 30.0f;
    
    // Discovery status
    renderer_->RenderText("+ - Discovered", legend_x, legend_y, 0.8f, glm::vec3(0.0f, 1.0f, 0.0f));
    legend_y += 25.0f;
    
    renderer_->RenderText("? - Undiscovered", legend_x, legend_y, 0.8f, glm::vec3(0.5f, 0.5f, 0.5f));
    legend_y += 40.0f;
    
    // Rarity colors
    renderer_->RenderText("Rarity Colors:", legend_x, legend_y, 1.0f, TEXT_COLOR);
    legend_y += 30.0f;
    
    std::vector<std::pair<std::string, glm::vec3>> rarity_info = {
        {"Common", COMMON_COLOR},
        {"Uncommon", UNCOMMON_COLOR},
        {"Rare", RARE_COLOR},
        {"Epic", EPIC_COLOR},
        {"Legendary", LEGENDARY_COLOR}
    };
    
    for (const auto& rarity : rarity_info) {
        renderer_->RenderText(rarity.first, legend_x, legend_y, 0.8f, rarity.second);
        legend_y += 25.0f;
    }
}

glm::vec3 InventoryUI::GetRarityColor(ItemRarity rarity) const {
    switch (rarity) {
        case ItemRarity::Common: return COMMON_COLOR;
        case ItemRarity::Uncommon: return UNCOMMON_COLOR;
        case ItemRarity::Rare: return RARE_COLOR;
        case ItemRarity::Epic: return EPIC_COLOR;
        case ItemRarity::Legendary: return LEGENDARY_COLOR;
        default: return UNDISCOVERED_COLOR;
    }
}

std::string InventoryUI::GetRarityName(ItemRarity rarity) const {
    switch (rarity) {
        case ItemRarity::Common: return "Common";
        case ItemRarity::Uncommon: return "Uncommon";
        case ItemRarity::Rare: return "Rare";
        case ItemRarity::Epic: return "Epic";
        case ItemRarity::Legendary: return "Legendary";
        default: return "Unknown";
    }
}

std::string InventoryUI::GetStatName(ItemStat stat) const {
    switch (stat) {
        case ItemStat::Damage: return "Damage";
        case ItemStat::FireRate: return "Fire Rate";
        case ItemStat::Range: return "Range";
        case ItemStat::Special: return "Special";
        default: return "Unknown";
    }
}

glm::vec2 InventoryUI::GetCellPosition(int row, int col) const {
    float x = GRID_START_X + col * (CELL_SIZE + CELL_SPACING);
    float y = GRID_START_Y + row * (CELL_SIZE + CELL_SPACING);
    return glm::vec2(x, y);
}

glm::vec2 InventoryUI::GetCellSize() const {
    return glm::vec2(CELL_SIZE, CELL_SIZE);
}

bool InventoryUI::IsPointInCell(const glm::vec2& point, int row, int col) const {
    glm::vec2 cell_pos = GetCellPosition(row, col);
    glm::vec2 cell_size = GetCellSize();
    
    return point.x >= cell_pos.x && point.x <= cell_pos.x + cell_size.x &&
           point.y >= cell_pos.y && point.y <= cell_pos.y + cell_size.y;
}
