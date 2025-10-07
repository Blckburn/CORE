#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    Camera(const glm::vec3& position, const glm::vec3& target);
    
    void Update(float delta_time);
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    
    void SetPosition(const glm::vec3& position);
    void SetTarget(const glm::vec3& target);
    void SetZoom(float zoom);
    void SetAspect(float aspect);
    
    glm::vec3 GetPosition() const { return position_; }
    glm::vec3 GetTarget() const { return target_; }
    float GetZoom() const { return zoom_; }
    
    // Camera controls
    void Rotate(float delta_x, float delta_y);
    void SetRotation(float x, float y);
    
private:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;
    float zoom_;
    float rotation_x_;
    float rotation_y_;
    float aspect_ratio_;
    
    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
    
    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
    void UpdateCameraPosition();
};
