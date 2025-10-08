// Implementation of 3D ray casting
#include "ray_caster.h"
#include "camera.h"
#include <glm/gtc/matrix_inverse.hpp>

RayCaster::RayCaster() {
}

RayCaster::~RayCaster() {
}

bool RayCaster::Initialize() {
    return true;
}

glm::vec3 RayCaster::ScreenToWorldRay(const glm::vec2& screen_pos, 
                                      const Camera* camera,
                                      int screen_width, 
                                      int screen_height) {
    return GetRayDirection(screen_pos, camera, screen_width, screen_height);
}

glm::vec3 RayCaster::GetGroundIntersection(const glm::vec2& screen_pos,
                                           const Camera* camera,
                                           int screen_width,
                                           int screen_height,
                                           float ground_y) {
    // Get ray direction
    glm::vec3 ray_direction = GetRayDirection(screen_pos, camera, screen_width, screen_height);
    glm::vec3 ray_origin = camera->GetPosition();
    
    // Calculate intersection with ground plane (Y = ground_y)
    // Ray equation: P = ray_origin + t * ray_direction
    // Ground plane equation: Y = ground_y
    // So: ray_origin.y + t * ray_direction.y = ground_y
    // Therefore: t = (ground_y - ray_origin.y) / ray_direction.y
    
    if (glm::abs(ray_direction.y) < 0.001f) {
        // Ray is parallel to ground plane
        return glm::vec3(0.0f);
    }
    
    float t = (ground_y - ray_origin.y) / ray_direction.y;
    
    if (t < 0.0f) {
        // Intersection is behind camera
        return glm::vec3(0.0f);
    }
    
    return ray_origin + t * ray_direction;
}

glm::vec3 RayCaster::GetSphereIntersection(const glm::vec2& screen_pos,
                                           const Camera* camera,
                                           int screen_width,
                                           int screen_height,
                                           const glm::vec3& sphere_center,
                                           float sphere_radius) {
    // Get ray direction and origin
    glm::vec3 ray_direction = GetRayDirection(screen_pos, camera, screen_width, screen_height);
    glm::vec3 ray_origin = camera->GetPosition();
    
    // Ray-sphere intersection
    glm::vec3 oc = ray_origin - sphere_center;
    float a = glm::dot(ray_direction, ray_direction);
    float b = 2.0f * glm::dot(oc, ray_direction);
    float c = glm::dot(oc, oc) - sphere_radius * sphere_radius;
    
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0.0f) {
        // No intersection
        return glm::vec3(0.0f);
    }
    
    float t1 = (-b - glm::sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
    
    float t = (t1 > 0.0f) ? t1 : t2;
    
    if (t <= 0.0f) {
        // Intersection is behind camera
        return glm::vec3(0.0f);
    }
    
    return ray_origin + t * ray_direction;
}

glm::vec3 RayCaster::UnprojectPoint(const glm::vec3& screen_point,
                                   const glm::mat4& view_matrix,
                                   const glm::mat4& projection_matrix,
                                   int screen_width,
                                   int screen_height) {
    // Convert screen coordinates to normalized device coordinates
    glm::vec4 ndc;
    ndc.x = (2.0f * screen_point.x) / screen_width - 1.0f;
    ndc.y = 1.0f - (2.0f * screen_point.y) / screen_height;
    ndc.z = 2.0f * screen_point.z - 1.0f;
    ndc.w = 1.0f;
    
    // Transform to world coordinates
    glm::mat4 inv_view_proj = glm::inverse(projection_matrix * view_matrix);
    glm::vec4 world_point = inv_view_proj * ndc;
    
    // Perspective divide
    if (world_point.w != 0.0f) {
        world_point /= world_point.w;
    }
    
    return glm::vec3(world_point);
}

glm::vec3 RayCaster::GetRayDirection(const glm::vec2& screen_pos,
                                     const Camera* camera,
                                     int screen_width,
                                     int screen_height) {
    // Create two points in screen space (near and far)
    glm::vec3 near_point(screen_pos.x, screen_pos.y, 0.0f);
    glm::vec3 far_point(screen_pos.x, screen_pos.y, 1.0f);
    
    // Unproject to world space
    glm::vec3 world_near = UnprojectPoint(near_point, 
                                         camera->GetViewMatrix(), 
                                         camera->GetProjectionMatrix(),
                                         screen_width, 
                                         screen_height);
    
    glm::vec3 world_far = UnprojectPoint(far_point, 
                                        camera->GetViewMatrix(), 
                                        camera->GetProjectionMatrix(),
                                        screen_width, 
                                        screen_height);
    
    // Return normalized direction
    return glm::normalize(world_far - world_near);
}

glm::vec3 RayCaster::GetPlaneIntersection(const glm::vec2& screen_pos,
                                         const Camera* camera,
                                         int screen_width,
                                         int screen_height,
                                         const glm::vec3& plane_center,
                                         const glm::vec3& plane_normal) {
    // Get ray direction and origin
    glm::vec3 ray_direction = GetRayDirection(screen_pos, camera, screen_width, screen_height);
    glm::vec3 ray_origin = camera->GetPosition();
    
    // Ray-plane intersection
    // Plane equation: (P - plane_center) · plane_normal = 0
    // Ray equation: P = ray_origin + t * ray_direction
    // Substituting: (ray_origin + t * ray_direction - plane_center) · plane_normal = 0
    // Solving for t: t = ((plane_center - ray_origin) · plane_normal) / (ray_direction · plane_normal)
    
    float denominator = glm::dot(ray_direction, plane_normal);
    
    if (glm::abs(denominator) < 0.001f) {
        // Ray is parallel to plane
        return glm::vec3(0.0f);
    }
    
    glm::vec3 to_plane = plane_center - ray_origin;
    float t = glm::dot(to_plane, plane_normal) / denominator;
    
    if (t < 0.0f) {
        // Intersection is behind camera
        return glm::vec3(0.0f);
    }
    
    return ray_origin + t * ray_direction;
}
