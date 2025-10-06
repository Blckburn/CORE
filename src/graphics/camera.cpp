#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() 
    : position_(0.0f, 0.0f, 25.0f)
    , target_(0.0f, 0.0f, 0.0f)
    , up_(0.0f, 1.0f, 0.0f)
    , zoom_(25.0f)
    , rotation_x_(0.0f)
    , rotation_y_(0.0f) {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

Camera::Camera(const glm::vec3& position, const glm::vec3& target)
    : position_(position)
    , target_(target)
    , up_(0.0f, 1.0f, 0.0f)
    , zoom_(glm::length(position - target)) {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera::Update(float delta_time) {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

glm::mat4 Camera::GetViewMatrix() const {
    return view_matrix_;
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return projection_matrix_;
}

void Camera::SetPosition(const glm::vec3& position) {
    position_ = position;
    zoom_ = glm::length(position - target_);
}

void Camera::SetTarget(const glm::vec3& target) {
    target_ = target;
    zoom_ = glm::length(position_ - target);
}

void Camera::SetZoom(float zoom) {
    zoom = glm::max(15.0f, glm::min(40.0f, zoom)); // Clamp zoom
    zoom_ = zoom;
    
    // Update camera position based on current rotation
    UpdateCameraPosition();
}

void Camera::Rotate(float delta_x, float delta_y) {
    rotation_x_ += delta_x * 0.005f; // Sensitivity
    rotation_y_ += delta_y * 0.005f;
    
    // Clamp vertical rotation
    rotation_y_ = glm::max(-1.5f, glm::min(1.5f, rotation_y_));
    
    UpdateCameraPosition();
}

void Camera::SetRotation(float x, float y) {
    rotation_x_ = x;
    rotation_y_ = glm::max(-1.5f, glm::min(1.5f, y));
    
    UpdateCameraPosition();
}

void Camera::UpdateCameraPosition() {
    // Calculate position based on rotation around target
    float x = glm::cos(rotation_x_) * glm::cos(rotation_y_);
    float y = glm::sin(rotation_y_);
    float z = glm::sin(rotation_x_) * glm::cos(rotation_y_);
    
    position_ = target_ + glm::vec3(x, y, z) * zoom_;
    
    // Update matrices after position change
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix() {
    view_matrix_ = glm::lookAt(position_, target_, up_);
}

void Camera::UpdateProjectionMatrix() {
    // For now, use a fixed aspect ratio and FOV
    // TODO: Get aspect ratio from window
    float aspect = 16.0f / 9.0f; // Default aspect ratio
    float fov = 45.0f;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;
    
    projection_matrix_ = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
}
