// Visual preview for turret placement (green/red indicator)
#pragma once

#include <glm/glm.hpp>
#include <memory>

class Mesh;
class Shader;

class TurretPreview {
public:
    TurretPreview();
    ~TurretPreview();

    // Initialize preview system
    bool Initialize();

    // Update preview position
    void Update(const glm::vec3& position, bool is_valid_placement);

    // Render preview
    void Render(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);

    // Control visibility
    void Show() { visible_ = true; }
    void Hide() { visible_ = false; }
    bool IsVisible() const { return visible_; }

    // Getters
    glm::vec3 GetPosition() const { return position_; }
    bool IsValidPlacement() const { return valid_placement_; }

    // Setters
    void SetValidPlacement(bool valid) { valid_placement_ = valid; }

private:
    glm::vec3 position_;
    bool valid_placement_;
    bool visible_;
    bool initialized_;

    // Graphics objects
    std::unique_ptr<Mesh> preview_mesh_;
    Shader* shader_;

    // Colors
    glm::vec3 valid_color_;
    glm::vec3 invalid_color_;
};

