#include "game.h"
#include "graphics/renderer.h"
#include "core/input.h"
#include "graphics/shader.h"
#include "graphics/mesh.h"
#include "graphics/camera.h"
#include "core/time.h"
#include "enemy_spawner.h"
#include "turret_manager.h"
#include "graphics/ray_caster.h"
#include "turret_preview.h"
#include "projectile_manager.h"
#include "projectile.h"
#include "wave_manager.h"
#include "enemy.h"
#include "ui_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>

// GLFW constants
#ifndef GLFW_MOUSE_BUTTON_LEFT
#define GLFW_MOUSE_BUTTON_LEFT 0
#endif

Game::Game() : renderer_(nullptr), input_(nullptr), initialized_(false) {
}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize(Renderer* renderer, InputManager* input) {
    std::cout << "Initializing game..." << std::endl;
    
    if (!renderer) {
        std::cerr << "Game: Renderer is null!" << std::endl;
        return false;
    }
    
    if (!input) {
        std::cerr << "Game: Input manager is null!" << std::endl;
        return false;
    }
    
    renderer_ = renderer;
    input_ = input;
    
    // Initialize camera
    camera_ = std::make_unique<Camera>();
    
    // Initialize shader
    shader_ = std::make_unique<Shader>();
    if (!shader_->LoadFromFiles("assets/shaders/basic.vert", "assets/shaders/basic.frag")) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return false;
    }
    
    // Initialize cube mesh (wireframe)
    cube_mesh_ = std::make_unique<Mesh>();
    cube_mesh_->CreateCubeWireframe();
    
    // Initialize enemy mesh (wireframe)
    enemy_mesh_ = std::make_unique<Mesh>();
    enemy_mesh_->CreateCubeWireframe();
    
    // Initialize turret mesh (wireframe)
    turret_mesh_ = std::make_unique<Mesh>();
    turret_mesh_->CreateCubeWireframe();
    
    // Initialize projectile mesh (disc)
    projectile_mesh_ = std::make_unique<Mesh>();
    projectile_mesh_->CreateDisc(0.5f, 16); // radius 0.5, 16 segments (увеличили размер)
    
    // Initialize enemy spawner
    enemy_spawner_ = std::make_unique<EnemySpawner>();
    if (!enemy_spawner_->Initialize()) {
        std::cerr << "Failed to initialize enemy spawner!" << std::endl;
        return false;
    }
    
    // Start spawning enemies
    enemy_spawner_->StartSpawning();
    enemy_spawner_->SetSpawnRate(0.5f); // 1 enemy every 2 seconds
    enemy_spawner_->SetSpawnRadius(30.0f); // Spawn 30 units from center
    
    // Initialize turret manager
    turret_manager_ = std::make_unique<TurretManager>();
    if (!turret_manager_->Initialize()) {
        std::cerr << "Failed to initialize turret manager!" << std::endl;
        return false;
    }
    
    // Initialize ray caster
    ray_caster_ = std::make_unique<RayCaster>();
    if (!ray_caster_->Initialize()) {
        std::cerr << "Failed to initialize ray caster!" << std::endl;
        return false;
    }
    
    // Initialize turret preview
    turret_preview_ = std::make_unique<TurretPreview>();
    if (!turret_preview_->Initialize()) {
        std::cerr << "Failed to initialize turret preview!" << std::endl;
        return false;
    }
    
    // Initialize projectile manager
    projectile_manager_ = std::make_unique<ProjectileManager>();
    if (!projectile_manager_->Initialize()) {
        std::cerr << "Failed to initialize projectile manager!" << std::endl;
        return false;
    }
    
    // Connect turret manager to projectile manager
    turret_manager_->SetProjectileManager(projectile_manager_.get());
    
    // Initialize wave manager
    wave_manager_ = std::make_unique<WaveManager>();
    wave_manager_->SetEnemySpawner(enemy_spawner_.get());
    
    // Connect enemy spawner and projectile manager to wave manager
    enemy_spawner_->SetWaveManager(wave_manager_.get());
    projectile_manager_->SetWaveManager(wave_manager_.get());
    
    // Теперь спавн контролируется системой волн, отключаем автоматический спавн
    enemy_spawner_->StopSpawning();
    
    // Запускаем игру с первой волной
    wave_manager_->StartGame();
    
    // Initialize UI manager
    ui_manager_ = std::make_unique<UIManager>();
    if (!ui_manager_->Initialize(shader_.get())) {
        std::cerr << "Failed to initialize UI manager!" << std::endl;
        return false;
    }
    
    // Initialize placement state
    turret_placement_mode_ = false;
    preview_position_ = glm::vec3(0.0f);
    preview_valid_ = false;
    
    initialized_ = true;
    std::cout << "Game initialized successfully!" << std::endl;
    std::cout << "=== CONTROLS ===" << std::endl;
    std::cout << "Right Mouse Button (hold): Rotate camera" << std::endl;
    std::cout << "WASD: Rotate camera" << std::endl;
    std::cout << "Q/E or Mouse Wheel: Zoom" << std::endl;
    std::cout << "T key: Toggle turret placement mode" << std::endl;
    std::cout << "Left Click: Place turret (when in placement mode)" << std::endl;
    std::cout << "+/- keys: Adjust placement distance (in placement mode)" << std::endl;
    std::cout << "=================" << std::endl;
    
    return true;
}

void Game::Update() {
    if (!initialized_) return;
    
    // Handle mouse input for camera - only when right mouse button is held
    if (input_->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        glm::vec2 mouse_delta = input_->GetMouseDelta();
        if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f) {
            camera_->Rotate(mouse_delta.x, mouse_delta.y);
        }
    }
    
    // Handle scroll for zoom
    float scroll_delta = input_->GetScrollDelta();
    if (scroll_delta != 0.0f) {
        float new_zoom = camera_->GetZoom() - scroll_delta * 3.0f;
        camera_->SetZoom(new_zoom);
        input_->ConsumeScrollDelta(); // Consume the scroll delta
    }
    
    // Handle keyboard for testing - use continuous input for smooth movement
    if (input_->IsKeyPressed(65)) { // GLFW_KEY_A
        camera_->Rotate(-10.0f, 0.0f);
    }
    if (input_->IsKeyPressed(68)) { // GLFW_KEY_D
        camera_->Rotate(10.0f, 0.0f);
    }
    if (input_->IsKeyPressed(87)) { // GLFW_KEY_W
        camera_->Rotate(0.0f, 5.0f);
    }
    if (input_->IsKeyPressed(83)) { // GLFW_KEY_S
        camera_->Rotate(0.0f, -5.0f);
    }
    
    // Zoom with keyboard - use continuous input
    if (input_->IsKeyPressed(81)) { // GLFW_KEY_Q
        camera_->SetZoom(camera_->GetZoom() + 2.0f);
    }
    if (input_->IsKeyPressed(69)) { // GLFW_KEY_E
        camera_->SetZoom(camera_->GetZoom() - 2.0f);
    }
    
    // Update camera
    camera_->Update(Time::GetDeltaTime());
    
    // Update enemy spawner and enemies
    if (enemy_spawner_) {
        enemy_spawner_->Update(Time::GetDeltaTime());
    }
    
    // Update turret manager and turrets
    if (turret_manager_ && enemy_spawner_) {
        const auto& enemies = enemy_spawner_->GetEnemies();
        turret_manager_->Update(Time::GetDeltaTime(), enemies);
    }
    
    // Handle turret placement system
    static bool left_button_was_pressed = false;
    bool left_button_is_pressed = input_->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
    
    // Toggle placement mode with T key
    if (input_->IsKeyJustPressed(84)) { // GLFW_KEY_T
        turret_placement_mode_ = !turret_placement_mode_;
        if (turret_placement_mode_) {
            turret_preview_->Show();
            std::cout << "Turret placement mode ON" << std::endl;
        } else {
            turret_preview_->Hide();
            std::cout << "Turret placement mode OFF" << std::endl;
        }
    }
    
    // Update preview position when in placement mode
    if (turret_placement_mode_) {
        glm::vec2 mouse_pos = input_->GetMousePosition();
        
        // Debug: Show mouse position and camera info
        static int preview_debug_counter = 0;
        preview_debug_counter++;
        if (preview_debug_counter % 60 == 0) { // Every second at 60 FPS
            std::cout << "Preview Debug: Mouse pos = (" << mouse_pos.x << ", " << mouse_pos.y 
                      << "), Camera pos = (" << camera_->GetPosition().x << ", " 
                      << camera_->GetPosition().y << ", " << camera_->GetPosition().z << ")" << std::endl;
        }
        
        // Get intersection with a plane that's perpendicular to camera direction
        // This allows placement in 3D space, not just on Y=0
        glm::vec3 camera_pos = camera_->GetPosition();
        glm::vec3 camera_direction = glm::normalize(camera_->GetTarget() - camera_pos);
        
        // Create a plane at a configurable distance from camera, perpendicular to camera direction
        static float placement_distance = 15.0f; // Distance from camera to placement plane
        
        // Allow changing placement distance with +/- keys
        if (input_->IsKeyPressed(61)) { // = key (same as +)
            placement_distance += 1.0f * Time::GetDeltaTime() * 10.0f;
            if (placement_distance > 30.0f) placement_distance = 30.0f;
        }
        if (input_->IsKeyPressed(45)) { // - key
            placement_distance -= 1.0f * Time::GetDeltaTime() * 10.0f;
            if (placement_distance < 5.0f) placement_distance = 5.0f;
        }
        
        glm::vec3 plane_center = camera_pos + camera_direction * placement_distance;
        glm::vec3 plane_normal = camera_direction;
        
        // Get intersection with this 3D plane
        glm::vec3 plane_intersection = ray_caster_->GetPlaneIntersection(
            mouse_pos, camera_.get(), 3840, 2054, plane_center, plane_normal);
        
        // Debug: Show intersection result
        if (preview_debug_counter % 60 == 0) {
            std::cout << "Plane intersection = (" << plane_intersection.x << ", " 
                      << plane_intersection.y << ", " << plane_intersection.z << ")" << std::endl;
        }
        
        if (plane_intersection != glm::vec3(0.0f)) {
            preview_position_ = plane_intersection;
            
            // Check if placement is valid
            preview_valid_ = turret_manager_->IsValidPlacement(preview_position_);
            
            // Debug: Show placement validity
            if (preview_debug_counter % 60 == 0) {
                std::cout << "Placement valid = " << preview_valid_ << std::endl;
            }
            
            // Update preview
            turret_preview_->Update(preview_position_, preview_valid_);
        } else {
            // Debug: No intersection found
            if (preview_debug_counter % 60 == 0) {
                std::cout << "No ground intersection found!" << std::endl;
            }
        }
        
        // Place turret on left click
        if (left_button_is_pressed && !left_button_was_pressed) {
            std::cout << "Left click detected in placement mode!" << std::endl;
            std::cout << "Preview position: (" << preview_position_.x << ", " 
                      << preview_position_.y << ", " << preview_position_.z << ")" << std::endl;
            std::cout << "Preview valid: " << preview_valid_ << std::endl;
            
            if (preview_valid_) {
                std::cout << "Attempting to place turret..." << std::endl;
                if (turret_manager_->PlaceTurret(preview_position_)) {
                    std::cout << "SUCCESS: Turret placed at: " 
                              << preview_position_.x << ", " 
                              << preview_position_.y << ", " 
                              << preview_position_.z << std::endl;
                    // Optionally exit placement mode after placing
                    // turret_placement_mode_ = false;
                    // turret_preview_->Hide();
                } else {
                    std::cout << "FAILED: TurretManager rejected placement" << std::endl;
                }
            } else {
                std::cout << "Cannot place turret at this location (preview not valid)" << std::endl;
            }
        }
    }
    
    left_button_was_pressed = left_button_is_pressed;
    
    // Update wave manager (контролирует спавн врагов)
    wave_manager_->Update(Time::GetDeltaTime());
    
    // Update game systems
    enemy_spawner_->Update(Time::GetDeltaTime());
    turret_manager_->Update(Time::GetDeltaTime(), enemy_spawner_->GetEnemies());
    projectile_manager_->Update(Time::GetDeltaTime(), enemy_spawner_->GetEnemies());
}

void Game::Render() {
    if (!initialized_) return;
    
    // Use shader
    shader_->Use();
    
    // Set up matrices
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    shader_->SetUniform("model", model);
    shader_->SetUniform("view", camera_->GetViewMatrix());
    shader_->SetUniform("projection", camera_->GetProjectionMatrix());
    
    // Set cube color (bright cyan for player core)
    shader_->SetUniform("color", glm::vec3(0.0f, 1.0f, 1.0f)); // Cyan
    
    // Render the cube as wireframe
    cube_mesh_->RenderWireframe();
    
    // Render enemies
    if (enemy_spawner_) {
        const auto& enemies = enemy_spawner_->GetEnemies();
        for (const auto& enemy : enemies) {
            if (enemy && enemy->IsAlive()) {
                // Set enemy position
                glm::mat4 enemy_model = glm::translate(glm::mat4(1.0f), enemy->GetPosition());
                shader_->SetUniform("model", enemy_model);
                
                // Set enemy color (red)
                shader_->SetUniform("color", enemy->GetColor());
                
                // Render enemy cube as wireframe
                enemy_mesh_->RenderWireframe();
            }
        }
    }
    
    // Render turrets
    if (turret_manager_) {
        const auto& turrets = turret_manager_->GetTurrets();
        for (const auto& turret : turrets) {
            if (turret && turret->IsActive()) {
                // Set turret position and rotation
                glm::mat4 turret_model = glm::mat4(1.0f);
                turret_model = glm::translate(turret_model, turret->GetPosition());
                turret_model = glm::rotate(turret_model, glm::radians(turret->GetRotation()), glm::vec3(0.0f, 1.0f, 0.0f));
                shader_->SetUniform("model", turret_model);
                
                // Set turret color (green)
                shader_->SetUniform("color", turret->GetColor());
                
                // Render turret cube as wireframe
                turret_mesh_->RenderWireframe();
            }
        }
    }
    
    // Render projectiles
    if (projectile_manager_) {
        const auto& projectiles = projectile_manager_->GetProjectiles();
        static int projectile_debug_counter = 0;
        projectile_debug_counter++;
        
        if (projectile_debug_counter % 60 == 0 && !projectiles.empty()) {
            std::cout << "Rendering " << projectiles.size() << " projectiles" << std::endl;
        }
        
        for (const auto& projectile : projectiles) {
            if (projectile && projectile->IsActive()) {
                // Set projectile position
                glm::mat4 projectile_model = glm::mat4(1.0f);
                projectile_model = glm::translate(projectile_model, projectile->GetPosition());
                shader_->SetUniform("model", projectile_model);
                
                // Set projectile color (cyan like in TRON)
                shader_->SetUniform("color", projectile->GetColor());
                
                // Render projectile disc as wireframe (hollow ring)
                projectile_mesh_->RenderWireframe();
            }
        }
    }
    
    // Render turret preview
    if (turret_preview_ && turret_preview_->IsVisible()) {
        // Set up model matrix for preview position
        glm::mat4 preview_model = glm::translate(glm::mat4(1.0f), preview_position_);
        shader_->SetUniform("model", preview_model);
        
        // Set preview color based on validity
        glm::vec3 preview_color = preview_valid_ ? 
            glm::vec3(0.0f, 1.0f, 0.0f) : // Green for valid
            glm::vec3(1.0f, 0.0f, 0.0f);  // Red for invalid
        shader_->SetUniform("color", preview_color);
        
        // Render preview as wireframe
        turret_mesh_->RenderWireframe();
    }
    
    // Render UI (last, on top of everything)
    if (ui_manager_ && wave_manager_) {
        ui_manager_->Render(wave_manager_.get(), 1280, 720);
    }
}

void Game::Shutdown() {
    std::cout << "Shutting down game..." << std::endl;
    
    // Clean up graphics objects
    cube_mesh_.reset();
    enemy_mesh_.reset();
    turret_mesh_.reset();
    projectile_mesh_.reset();
    shader_.reset();
    camera_.reset();
    enemy_spawner_.reset();
    turret_manager_.reset();
    ray_caster_.reset();
    turret_preview_.reset();
    projectile_manager_.reset();
    wave_manager_.reset();
    ui_manager_.reset();
    
    initialized_ = false;
    renderer_ = nullptr;
    input_ = nullptr;
}
