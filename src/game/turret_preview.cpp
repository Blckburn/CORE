// Implementation of visual turret placement preview
#include "turret_preview.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

TurretPreview::TurretPreview() :
    position_(0.0f),
    valid_placement_(true),
    visible_(false),
    initialized_(false),
    shader_(nullptr),
    valid_color_(0.0f, 1.0f, 0.0f),    // Green for valid
    invalid_color_(1.0f, 0.0f, 0.0f) { // Red for invalid
}

TurretPreview::~TurretPreview() {
}

bool TurretPreview::Initialize() {
    std::cout << "Initializing turret preview..." << std::endl;
    
    // Create preview mesh (wireframe cube)
    preview_mesh_ = std::make_unique<Mesh>();
    preview_mesh_->CreateCubeWireframe();
    
    initialized_ = true;
    return true;
}

void TurretPreview::Update(const glm::vec3& position, bool is_valid_placement) {
    if (!initialized_) return;
    
    position_ = position;
    valid_placement_ = is_valid_placement;
}

void TurretPreview::Render(const glm::mat4& view_matrix, const glm::mat4& projection_matrix) {
    if (!initialized_ || !visible_) return;
    
    // Note: Shader should be set by the calling code before calling this method
    // We'll assume the shader is already bound and uniforms are set by the caller
    
    // Set up model matrix for preview position
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position_);
    
    // Set color based on validity
    glm::vec3 color = valid_placement_ ? valid_color_ : invalid_color_;
    
    // These uniforms need to be set by the calling code:
    // shader_->SetUniform("model", model);
    // shader_->SetUniform("color", color);
    
    // For now, we'll just render the mesh
    // The caller should set the uniforms before calling this method
    preview_mesh_->RenderWireframe();
}
