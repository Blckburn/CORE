#pragma once

#include <glm/glm.hpp>

namespace Math {
    // Convert screen coordinates to world coordinates
    glm::vec3 ScreenToWorld(const glm::vec2& screen_pos, 
                           const glm::mat4& view_matrix,
                           const glm::mat4& projection_matrix,
                           float depth = 0.0f);
    
    // Get ray direction from screen position
    glm::vec3 GetRayDirection(const glm::vec2& screen_pos,
                             const glm::mat4& view_matrix,
                             const glm::mat4& projection_matrix);
    
    // Check if ray intersects with cube
    bool RayIntersectsCube(const glm::vec3& ray_origin,
                          const glm::vec3& ray_direction,
                          const glm::vec3& cube_center,
                          float cube_size);
    
    // Generate random position on sphere surface
    glm::vec3 RandomPositionOnSphere(float radius);
}
