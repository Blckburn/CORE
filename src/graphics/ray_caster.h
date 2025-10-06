#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera;

class RayCaster {
public:
    RayCaster();
    ~RayCaster();

    // Initialize ray caster
    bool Initialize();

    // Convert screen coordinates to world ray
    glm::vec3 ScreenToWorldRay(const glm::vec2& screen_pos, 
                              const Camera* camera,
                              int screen_width, 
                              int screen_height);

    // Get intersection point with ground plane (Y = 0)
    glm::vec3 GetGroundIntersection(const glm::vec2& screen_pos,
                                   const Camera* camera,
                                   int screen_width,
                                   int screen_height,
                                   float ground_y = 0.0f);

    // Get intersection point with sphere
    glm::vec3 GetSphereIntersection(const glm::vec2& screen_pos,
                                   const Camera* camera,
                                   int screen_width,
                                   int screen_height,
                                   const glm::vec3& sphere_center,
                                   float sphere_radius);

    // Get intersection point with arbitrary plane
    glm::vec3 GetPlaneIntersection(const glm::vec2& screen_pos,
                                  const Camera* camera,
                                  int screen_width,
                                  int screen_height,
                                  const glm::vec3& plane_center,
                                  const glm::vec3& plane_normal);

private:
    // Helper functions
    glm::vec3 UnprojectPoint(const glm::vec3& screen_point,
                            const glm::mat4& view_matrix,
                            const glm::mat4& projection_matrix,
                            int screen_width,
                            int screen_height);
    
    glm::vec3 GetRayDirection(const glm::vec2& screen_pos,
                             const Camera* camera,
                             int screen_width,
                             int screen_height);
};
