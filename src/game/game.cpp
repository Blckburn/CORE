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
    // Пробуем разные пути к шейдерам
    std::vector<std::string> shader_paths = {
        "assets/shaders/",
        "../assets/shaders/",
        "../../assets/shaders/",
        "build/assets/shaders/"
    };
    
    bool shader_loaded = false;
    for (const auto& path : shader_paths) {
        std::string vert_path = path + "basic.vert";
        std::string frag_path = path + "basic.frag";
        if (shader_->LoadFromFiles(vert_path, frag_path)) {
            std::cout << "Shaders loaded successfully from: " << path << std::endl;
            shader_loaded = true;
            break;
        }
    }
    
    if (!shader_loaded) {
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
    
    // Начинаем с главного меню, игра на паузе
    state_ = GameState::MainMenu;
    paused_ = true;
    // Инициализируем, но не запускаем волны до старта игры
    
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
    
    // Initialize turret management state
    selected_turret_ = nullptr;
    turret_menu_open_ = false;
    turret_menu_position_ = glm::vec3(0.0f);
    
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
    
    // Handle mouse input for camera - only when right mouse button is held for a while
    // This prevents camera rotation on quick right-clicks (for turret selection)
    static float camera_rotate_hold_time = 0.0f;
    if (input_->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        camera_rotate_hold_time += Time::GetDeltaTime();
        // Only rotate camera if button held for more than 0.05 seconds
        if (camera_rotate_hold_time > 0.05f) {
            glm::vec2 mouse_delta = input_->GetMouseDelta();
            if (mouse_delta.x != 0.0f || mouse_delta.y != 0.0f) {
                camera_->Rotate(mouse_delta.x, mouse_delta.y);
            }
        }
    } else {
        camera_rotate_hold_time = 0.0f;
    }
    
    // Handle scroll for zoom
    float scroll_delta = input_->GetScrollDelta();
    if (scroll_delta != 0.0f) {
        float new_zoom = camera_->GetZoom() - scroll_delta * 3.0f;
        camera_->SetZoom(new_zoom);
        input_->ConsumeScrollDelta(); // Consume the scroll delta
    }
    
    // Handle keyboard for testing - use continuous input for smooth movement
    // Check for game over
    if (state_ == GameState::Playing && wave_manager_ && wave_manager_->IsGameOver()) {
        state_ = GameState::GameOver;
        std::cout << "=== GAME OVER ===" << std::endl;
    }
    
    // Menu navigation
    if (state_ == GameState::MainMenu) {
        int &menu_index = main_menu_index_;
        if (input_->IsKeyJustPressed(265)) { menu_index = (menu_index + 2) % 3; }
        if (input_->IsKeyJustPressed(264)) { menu_index = (menu_index + 1) % 3; }
        int w = renderer_ ? renderer_->GetViewportWidth() : 1280;
        int h = renderer_ ? renderer_->GetViewportHeight() : 720;
        // Mouse hover detection
        glm::vec2 mouse = input_->GetMousePosition();
        float cx = w / 2.0f - 100.0f;
        float cy = h / 2.0f - 60.0f;
        
        for (int i = 0; i < 3; ++i) {
            float rx = cx - 12.0f;
            float ry = cy + i * 30.0f - 6.0f;
            float rw = 240.0f;
            float rh = 26.0f;
            if (mouse.x >= rx && mouse.x <= rx + rw && mouse.y >= ry && mouse.y <= ry + rh) {
                menu_index = i;
                if (input_->IsMouseButtonJustPressed(0)) { // LMB click
                    if (menu_index == 0) {
                        wave_manager_->StartGame();
                        state_ = GameState::Playing;
                        paused_ = false;
                    } else if (menu_index == 1) {
                        state_ = GameState::Options;
                    } else if (menu_index == 2) {
                        std::cout << "Exit requested from main menu" << std::endl;
                    }
                }
            }
        }
        if (input_->IsKeyJustPressed(257) || input_->IsKeyJustPressed(335)) { // Enter
            if (menu_index == 0) {
                wave_manager_->StartGame();
                state_ = GameState::Playing;
                paused_ = false;
            } else if (menu_index == 1) {
                state_ = GameState::Options;
            } else if (menu_index == 2) {
                std::cout << "Exit requested from main menu" << std::endl;
            }
        }
        return;
    }
    if (state_ == GameState::Options) {
        // Navigate options with Up/Down, apply with Enter, back with Esc
        if (input_->IsKeyJustPressed(265)) { // Up
            options_selected_index_ = std::max(0, options_selected_index_ - 1);
        }
        if (input_->IsKeyJustPressed(264)) { // Down
            options_selected_index_ = std::min(3, options_selected_index_ + 1);
        }
        // Mouse hover/click
        int w = renderer_ ? renderer_->GetViewportWidth() : 1280;
        int h = renderer_ ? renderer_->GetViewportHeight() : 720;
        glm::vec2 mouse = input_->GetMousePosition();
        float cx = w / 2.0f - 140.0f;
        float cy = h / 2.0f - 90.0f;
        for (int i = 0; i < 4; ++i) {
            float rx = cx - 12.0f;
            float ry = cy + i * 30.0f - 6.0f;
            float rw = 300.0f;
            float rh = 26.0f;
            if (mouse.x >= rx && mouse.x <= rx + rw && mouse.y >= ry && mouse.y <= ry + rh) {
                options_selected_index_ = i;
                if (input_->IsMouseButtonJustPressed(0)) {
                    int widths[4] = {1280, 1920, 2560, 3840};
                    int heights[4] = {720, 1080, 1440, 2160};
                    int wset = widths[options_selected_index_];
                    int hset = heights[options_selected_index_];
                    if (renderer_) renderer_->SetWindowSize(wset, hset);
                }
            }
        }
        if (input_->IsKeyJustPressed(257) || input_->IsKeyJustPressed(335)) { // Enter
            int widths[4] = {1280, 1920, 2560, 3840};
            int heights[4] = {720, 1080, 1440, 2160};
            int w = widths[options_selected_index_];
            int h = heights[options_selected_index_];
            if (renderer_) renderer_->SetWindowSize(w, h);
        }
        if (input_->IsKeyJustPressed(256)) { // Esc
            state_ = GameState::MainMenu;
        }
        return;
    }
    
    // Game Over menu
    if (state_ == GameState::GameOver) {
        int &menu_index = game_over_menu_index_;
        if (input_->IsKeyJustPressed(265)) { menu_index = (menu_index + 1) % 2; } // Up
        if (input_->IsKeyJustPressed(264)) { menu_index = (menu_index + 1) % 2; } // Down
        int w = renderer_ ? renderer_->GetViewportWidth() : 1280;
        int h = renderer_ ? renderer_->GetViewportHeight() : 720;
        // Mouse hover detection
        glm::vec2 mouse = input_->GetMousePosition();
        float cx = w / 2.0f - 100.0f;
        float cy = h / 2.0f + 40.0f; // Below game over text
        for (int i = 0; i < 2; ++i) {
            float rx = cx - 12.0f;
            float ry = cy + i * 30.0f - 6.0f;
            float rw = 240.0f;
            float rh = 26.0f;
            if (mouse.x >= rx && mouse.x <= rx + rw && mouse.y >= ry && mouse.y <= ry + rh) {
                menu_index = i;
                if (input_->IsMouseButtonJustPressed(0)) {
                    if (menu_index == 0) { // Restart
                        std::cout << "Restarting game..." << std::endl;
                        // Reset game state
                        if (enemy_spawner_) enemy_spawner_->ClearAllEnemies();
                        if (turret_manager_) turret_manager_->ClearAllTurrets();
                        if (projectile_manager_) {
                            // Projectiles will be cleared automatically
                        }
                        turret_cost_ = 1; // Reset turret cost
                        wave_manager_->StartGame();
                        state_ = GameState::Playing;
                        paused_ = false;
                    } else if (menu_index == 1) { // Main Menu
                        std::cout << "Returning to main menu..." << std::endl;
                        state_ = GameState::MainMenu;
                        // Reset game
                        if (enemy_spawner_) enemy_spawner_->ClearAllEnemies();
                        if (turret_manager_) turret_manager_->ClearAllTurrets();
                        turret_cost_ = 1;
                    }
                }
            }
        }
        if (input_->IsKeyJustPressed(257) || input_->IsKeyJustPressed(335)) { // Enter
            if (menu_index == 0) { // Restart
                std::cout << "Restarting game..." << std::endl;
                if (enemy_spawner_) enemy_spawner_->ClearAllEnemies();
                if (turret_manager_) turret_manager_->ClearAllTurrets();
                turret_cost_ = 1;
                wave_manager_->StartGame();
                state_ = GameState::Playing;
                paused_ = false;
            } else if (menu_index == 1) { // Main Menu
                std::cout << "Returning to main menu..." << std::endl;
                state_ = GameState::MainMenu;
                if (enemy_spawner_) enemy_spawner_->ClearAllEnemies();
                if (turret_manager_) turret_manager_->ClearAllTurrets();
                turret_cost_ = 1;
            }
        }
        return;
    }
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

    // Toggle pause with P (edge trigger) when playing
    static bool p_key_was_pressed = false;
    bool p_key_is_pressed = input_->IsKeyPressed(80); // GLFW_KEY_P
    if (state_ == GameState::Playing && p_key_is_pressed && !p_key_was_pressed) {
        paused_ = !paused_;
        std::cout << (paused_ ? "Game paused" : "Game resumed") << std::endl;
    }
    p_key_was_pressed = p_key_is_pressed;

    // Hold R for 2 seconds to restart the game
    static float r_hold_time = 0.0f;
    if (input_->IsKeyPressed(82)) { // GLFW_KEY_R
        r_hold_time += Time::GetDeltaTime();
        if (r_hold_time > 2.0f) {
            std::cout << "\nRestarting game..." << std::endl;
            // Clear game state
            if (projectile_manager_) {
                // Remove active projectiles by recreating manager state
                // (simple approach: reset vector implicitly by recreating object not needed here)
            }
            if (enemy_spawner_) enemy_spawner_->ClearAllEnemies();
            if (turret_manager_) turret_manager_->ClearAllTurrets();
            // Reset waves
            if (wave_manager_) wave_manager_->StartGame();
            r_hold_time = 0.0f;
        }
    } else {
        r_hold_time = 0.0f;
    }
    
    // Update camera
    if (state_ == GameState::Playing && !paused_) camera_->Update(Time::GetDeltaTime());
    
    // Update enemy spawner and enemies
    if (state_ == GameState::Playing && !paused_ && enemy_spawner_) {
        enemy_spawner_->Update(Time::GetDeltaTime());
    }
    
    // Update turret manager and turrets
    if (state_ == GameState::Playing && !paused_ && turret_manager_ && enemy_spawner_) {
        const auto& enemies = enemy_spawner_->GetEnemies();
        turret_manager_->Update(Time::GetDeltaTime(), enemies);
    }
    
    // Handle turret placement system
    static bool left_button_was_pressed = false;
    bool left_button_is_pressed = input_->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
    
    // Toggle placement mode with T key (alternative method)
    static bool t_key_was_pressed = false;
    bool t_key_is_pressed = input_->IsKeyPressed(84); // GLFW_KEY_T
    if (t_key_is_pressed && !t_key_was_pressed) {
        turret_placement_mode_ = !turret_placement_mode_;
        if (turret_placement_mode_) {
            turret_preview_->Show();
            std::cout << "Turret placement mode ON (T key)" << std::endl;
        } else {
            turret_preview_->Hide();
            std::cout << "Turret placement mode OFF (T key)" << std::endl;
        }
    }
    t_key_was_pressed = t_key_is_pressed;
    
    // Update preview position when in placement mode
    if (state_ == GameState::Playing && turret_placement_mode_) {
        // Use framebuffer-space mouse position (future-proof for HiDPI)
        glm::vec2 mouse_pos = input_->GetMousePositionFramebuffer();
        
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
        // Use actual viewport size instead of hardcoded values
        int viewport_w = renderer_ ? renderer_->GetViewportWidth() : 1280;
        int viewport_h = renderer_ ? renderer_->GetViewportHeight() : 720;
        glm::vec3 plane_intersection = ray_caster_->GetPlaneIntersection(
            mouse_pos, camera_.get(), viewport_w, viewport_h, plane_center, plane_normal);
        
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
        if (!paused_ && left_button_is_pressed && !left_button_was_pressed) {
            std::cout << "Left click detected in placement mode!" << std::endl;
            std::cout << "Preview position: (" << preview_position_.x << ", " 
                      << preview_position_.y << ", " << preview_position_.z << ")" << std::endl;
            std::cout << "Preview valid: " << preview_valid_ << std::endl;
            
            if (preview_valid_) {
                // Check turret limit
                if (!turret_manager_->CanPlaceMoreTurrets()) {
                    std::cout << "Cannot place turret: limit reached!" << std::endl;
                } 
                // Economy: try spend currency
                else if (wave_manager_->SpendCurrency(turret_cost_)) {
                    std::cout << "Attempting to place turret..." << std::endl;
                    if (turret_manager_->PlaceTurret(preview_position_)) {
                    std::cout << "SUCCESS: Turret placed at: " 
                              << preview_position_.x << ", " 
                              << preview_position_.y << ", " 
                              << preview_position_.z << std::endl;
                    
                    // Set the cost of the turret we just placed
                    auto& turrets = turret_manager_->GetTurrets();
                    if (!turrets.empty() && turrets.back()) {
                        turrets.back()->SetCost(turret_cost_);
                    }
                    
                    // Increase turret cost for next turret (progressive pricing)
                    turret_cost_ += 1; // Each turret costs 1 more than the previous
                    std::cout << "Next turret will cost: " << turret_cost_ << std::endl;
                    // Optionally exit placement mode after placing
                    // turret_placement_mode_ = false;
                    // turret_preview_->Hide();
                    } else {
                        std::cout << "FAILED: TurretManager rejected placement" << std::endl;
                        // refund if failed
                        wave_manager_->AddCurrency(turret_cost_);
                    }
                } else {
                    std::cout << "Not enough currency to place turret (cost " << turret_cost_ << ")" << std::endl;
                }
            } else {
                std::cout << "Cannot place turret at this location (preview not valid)" << std::endl;
            }
        }
    }
    
    left_button_was_pressed = left_button_is_pressed;
    
    // Handle right mouse button click for turret selection (when NOT in placement mode)
    static bool right_button_was_pressed = false;
    static float right_button_hold_time = 0.0f;
    bool right_button_is_pressed = input_->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT);
    
    if (state_ == GameState::Playing && !paused_ && !turret_placement_mode_) {
        if (right_button_is_pressed) {
            right_button_hold_time += Time::GetDeltaTime();
        }
        
        // On release: if it was a quick click (< 0.15s), select turret
        if (!right_button_is_pressed && right_button_was_pressed && right_button_hold_time < 0.15f) {
            // Quick right click - try to select turret
            glm::vec2 mouse_pos = input_->GetMousePositionFramebuffer();
            int viewport_w = renderer_ ? renderer_->GetViewportWidth() : 1280;
            int viewport_h = renderer_ ? renderer_->GetViewportHeight() : 720;
            
            // Cast ray from mouse to 3D world
            glm::vec3 camera_pos = camera_->GetPosition();
            glm::vec3 camera_dir = glm::normalize(camera_->GetTarget() - camera_pos);
            float placement_distance = 15.0f;
            glm::vec3 plane_center = camera_pos + camera_dir * placement_distance;
            glm::vec3 plane_normal = camera_dir;
            glm::vec3 world_pos = ray_caster_->GetPlaneIntersection(
                mouse_pos, camera_.get(), viewport_w, viewport_h, plane_center, plane_normal);
            
            // Try to find turret at this position (increased radius to 4.0 for easier selection)
            Turret* turret = turret_manager_->GetTurretAtPosition(world_pos, 4.0f);
            if (turret) {
                selected_turret_ = turret;
                turret_menu_open_ = true;
                turret_menu_position_ = turret->GetPosition();
                std::cout << "Turret selected at: " << turret->GetPosition().x << ", " 
                          << turret->GetPosition().y << ", " << turret->GetPosition().z << std::endl;
            } else {
                // Clicked empty space - close menu
                selected_turret_ = nullptr;
                turret_menu_open_ = false;
            }
        }
        
        if (!right_button_is_pressed) {
            right_button_hold_time = 0.0f;
        }
    }
    right_button_was_pressed = right_button_is_pressed;
    
    // Update wave manager (контролирует спавн врагов)
    if (state_ == GameState::Playing && !paused_) wave_manager_->Update(Time::GetDeltaTime());
    
    // Update game systems
    if (state_ == GameState::Playing && !paused_) {
        enemy_spawner_->Update(Time::GetDeltaTime());
        turret_manager_->Update(Time::GetDeltaTime(), enemy_spawner_->GetEnemies());
        projectile_manager_->Update(Time::GetDeltaTime(), enemy_spawner_->GetEnemies());
    }
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
                
                // Highlight selected turret
                bool is_selected = (selected_turret_ == turret.get());
                glm::vec3 turret_color = is_selected ? 
                    glm::vec3(1.0f, 1.0f, 0.0f) : // Yellow when selected
                    turret->GetColor();            // Green normally
                
                shader_->SetUniform("color", turret_color);
                
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
        // Use actual viewport size
        int w = renderer_ ? renderer_->GetViewportWidth() : 1280;
        int h = renderer_ ? renderer_->GetViewportHeight() : 720;
        if (state_ == GameState::MainMenu) {
            ui_manager_->RenderMainMenu(w, h, main_menu_index_);
        } else if (state_ == GameState::Options) {
            ui_manager_->RenderOptionsMenu(w, h, options_selected_index_);
        } else if (state_ == GameState::GameOver) {
            ui_manager_->RenderGameOverMenu(w, h, game_over_menu_index_, wave_manager_.get());
        } else {
            ui_manager_->RenderWithTurrets(wave_manager_.get(), turret_manager_.get(), w, h);
        }
        if (paused_ && state_ == GameState::Playing) {
            ui_manager_->RenderPausedOverlay(w, h);
        }

        // Tooltip near cursor in placement mode: show turret cost with color by affordability
        if (state_ == GameState::Playing && turret_placement_mode_) {
            int money = wave_manager_->GetCurrency();
            const int turret_cost = turret_cost_;
            glm::vec3 tooltip_color = (money >= turret_cost) ? glm::vec3(0.0f, 1.0f, 0.0f)
                                                             : glm::vec3(1.0f, 0.0f, 0.0f);
            glm::vec2 mouse = input_->GetMousePosition();
            // Курсор координаты уже в пикселях окна (0..width, 0..height)
            float tip_x = mouse.x + 16.0f;
            float tip_y = mouse.y + 24.0f;
            ui_manager_->RenderTooltip("COST: " + std::to_string(turret_cost), tip_x, tip_y, 0.7f, tooltip_color);
        }
        
        // Turret menu when turret is selected
        if (state_ == GameState::Playing && turret_menu_open_ && selected_turret_) {
            bool sell_clicked = false;
            ui_manager_->RenderTurretMenu(turret_menu_position_, camera_.get(), input_, w, h, sell_clicked);
            
            // Handle sell click
            if (sell_clicked && selected_turret_) {
                // Calculate refund (50% of original cost)
                int original_cost = selected_turret_->GetCost();
                int refund = std::max(1, original_cost / 2); // At least 1 credit back
                
                std::cout << "Selling turret (cost: " << original_cost << ", refund: " << refund << ")" << std::endl;
                
                // Remove turret and refund money
                if (turret_manager_->RemoveTurretAtPosition(turret_menu_position_, 1.5f)) {
                    wave_manager_->AddCurrency(refund);
                    std::cout << "Turret sold for " << refund << " credits" << std::endl;
                    selected_turret_ = nullptr;
                    turret_menu_open_ = false;
                }
            }
        }
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
