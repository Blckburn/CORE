# Technical Architecture Specification: CORE
## Minimalist 3D Tower Defense Engine

---

## Project Overview

### Technology Stack
- **Language**: C++17
- **Graphics API**: OpenGL 3.3+ (Vulkan upgrade path planned)
- **Math Library**: GLM (OpenGL Mathematics)
- **Window Management**: GLFW
- **Audio**: OpenAL or FMOD (minimal requirements)
- **Build System**: CMake 3.20+
- **Platforms**: Windows, Linux, macOS

### Performance Targets
- **Frame Rate**: 60 FPS minimum
- **Memory Usage**: < 200 MB RAM
- **Load Time**: < 3 seconds
- **Binary Size**: < 50 MB
- **Target Hardware**: GTX 1060 / RX 580 equivalent

---

## Core Architecture

### Engine Structure
```
CORE/
├── src/
│   ├── core/                 # Engine core systems
│   │   ├── engine.h          # Main engine class
│   │   ├── window.h          # Window management
│   │   ├── input.h           # Input handling
│   │   └── time.h            # Time management
│   ├── graphics/             # Rendering systems
│   │   ├── renderer.h        # Main renderer
│   │   ├── shader.h          # Shader management
│   │   ├── mesh.h            # Mesh rendering
│   │   └── camera.h          # Camera system
│   ├── game/                 # Game logic
│   │   ├── game.h            # Main game class
│   │   ├── entity.h          # Entity system
│   │   ├── turret.h          # Turret logic
│   │   ├── enemy.h           # Enemy logic
│   │   └── projectile.h      # Projectile logic
│   └── utils/                # Utilities
│       ├── math.h            # Math utilities
│       └── debug.h           # Debug tools
├── assets/
│   ├── shaders/              # GLSL shaders
│   ├── textures/             # Simple textures (if needed)
│   └── audio/                # Sound effects
└── external/                 # Third-party libraries
```

### Entity Component System (ECS)

#### Entity Class
```cpp
class Entity {
public:
    Entity(uint32_t id) : id_(id) {}
    uint32_t GetID() const { return id_; }
    
private:
    uint32_t id_;
};
```

#### Component System
```cpp
// Position Component
struct PositionComponent {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

// Health Component
struct HealthComponent {
    float current_health;
    float max_health;
};

// Render Component
struct RenderComponent {
    glm::vec3 color;
    float glow_intensity;
    bool visible;
};

// Movement Component
struct MovementComponent {
    glm::vec3 velocity;
    glm::vec3 target_position;
    float speed;
};

// Turret Component
struct TurretComponent {
    float range;
    float fire_rate;
    float last_shot_time;
    Entity target;
};
```

#### System Classes
```cpp
class RenderSystem {
public:
    void Update(const std::vector<Entity>& entities);
    void Render(const std::vector<Entity>& entities);
    
private:
    void RenderCube(const PositionComponent& pos, 
                   const RenderComponent& render);
};

class MovementSystem {
public:
    void Update(std::vector<Entity>& entities, float delta_time);
    
private:
    void UpdateEnemyMovement(Entity entity, float delta_time);
};

class TurretSystem {
public:
    void Update(std::vector<Entity>& entities, float delta_time);
    void FireTurret(Entity turret, Entity target);
    
private:
    Entity FindNearestEnemy(Entity turret);
    bool IsInRange(Entity turret, Entity enemy);
};

class CollisionSystem {
public:
    void CheckCollisions(std::vector<Entity>& entities);
    bool CheckProjectileHit(Entity projectile, Entity target);
    bool CheckEnemyReachCore(Entity enemy);
};
```

---

## Rendering Pipeline

### Shader Architecture

#### Vertex Shader (basic.vert)
```glsl
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertexColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
}
```

#### Fragment Shader (basic.frag)
```glsl
#version 330 core

in vec3 vertexColor;
out vec4 FragColor;

uniform float glow_intensity;

void main() {
    vec3 finalColor = vertexColor * glow_intensity;
    FragColor = vec4(finalColor, 1.0);
}
```

### Mesh System
```cpp
class CubeMesh {
public:
    static void Create();
    static void Render();
    static void Destroy();
    
private:
    static GLuint VAO, VBO, EBO;
    static const float vertices[];
    static const unsigned int indices[];
};

// Cube vertices (1x1x1 unit cube)
const float CubeMesh::vertices[] = {
    // Front face
    -0.5f, -0.5f,  0.5f,  // Bottom-left
     0.5f, -0.5f,  0.5f,  // Bottom-right
     0.5f,  0.5f,  0.5f,  // Top-right
    -0.5f,  0.5f,  0.5f,  // Top-left
    // ... (complete cube definition)
};
```

### Camera System
```cpp
class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 target);
    
    void Update(float delta_time);
    void HandleInput(float delta_time);
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    
    void SetPosition(glm::vec3 position);
    void SetTarget(glm::vec3 target);
    void SetZoom(float zoom);
    
private:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;
    float zoom_;
    float rotation_speed_;
    float zoom_speed_;
    
    void UpdateViewMatrix();
    glm::mat4 view_matrix_;
    glm::mat4 projection_matrix_;
};
```

---

## Game Logic Architecture

### Game State Management
```cpp
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
public:
    Game();
    ~Game();
    
    void Initialize();
    void Run();
    void Shutdown();
    
    void Update(float delta_time);
    void Render();
    
    void HandleInput();
    void HandleMouseClick(glm::vec2 screen_pos);
    
private:
    GameState current_state_;
    float game_time_;
    int score_;
    int energy_;
    
    // Core systems
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<RenderSystem> render_system_;
    std::unique_ptr<MovementSystem> movement_system_;
    std::unique_ptr<TurretSystem> turret_system_;
    std::unique_ptr<CollisionSystem> collision_system_;
    
    // Game entities
    Entity core_entity_;
    std::vector<Entity> turrets_;
    std::vector<Entity> enemies_;
    std::vector<Entity> projectiles_;
    
    void SpawnEnemy();
    void UpdateGameplay(float delta_time);
    void CheckGameOver();
};
```

### Entity Management
```cpp
class EntityManager {
public:
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    
    template<typename T>
    void AddComponent(Entity entity, const T& component);
    
    template<typename T>
    T& GetComponent(Entity entity);
    
    template<typename T>
    bool HasComponent(Entity entity);
    
    template<typename T>
    void RemoveComponent(Entity entity);
    
    std::vector<Entity> GetEntitiesWithComponent();
    
private:
    uint32_t next_entity_id_;
    std::unordered_map<uint32_t, std::unordered_map<std::type_index, void*>> components_;
    
    template<typename T>
    std::type_index GetTypeIndex();
};
```

### Spatial Management
```cpp
class SpatialGrid {
public:
    SpatialGrid(float cell_size, glm::vec3 world_size);
    
    void Insert(Entity entity, const PositionComponent& pos);
    void Remove(Entity entity);
    void Update(Entity entity, const PositionComponent& old_pos, 
                const PositionComponent& new_pos);
    
    std::vector<Entity> GetEntitiesInRadius(glm::vec3 center, float radius);
    std::vector<Entity> GetEntitiesInBox(glm::vec3 min, glm::vec3 max);
    
private:
    float cell_size_;
    glm::vec3 world_size_;
    std::unordered_map<glm::ivec3, std::vector<Entity>> grid_;
    
    glm::ivec3 WorldToGrid(glm::vec3 world_pos);
    std::vector<glm::ivec3> GetCellsInRadius(glm::vec3 center, float radius);
};
```

---

## Input System

### Input Handler
```cpp
class InputManager {
public:
    InputManager(GLFWwindow* window);
    
    void Update();
    
    bool IsKeyPressed(int key);
    bool IsMouseButtonPressed(int button);
    glm::vec2 GetMousePosition();
    glm::vec2 GetMouseDelta();
    float GetScrollDelta();
    
    void SetMouseCallback(std::function<void(int, int)> callback);
    void SetScrollCallback(std::function<void(float)> callback);
    
private:
    GLFWwindow* window_;
    glm::vec2 mouse_position_;
    glm::vec2 mouse_delta_;
    float scroll_delta_;
    
    static void MouseCallback(GLFWwindow* window, double x, double y);
    static void ScrollCallback(GLFWwindow* window, double x, double y);
    static void KeyCallback(GLFWwindow* window, int key, int scancode, 
                           int action, int mods);
};
```

### 3D Mouse Interaction
```cpp
class Mouse3D {
public:
    static glm::vec3 ScreenToWorld(glm::vec2 screen_pos, 
                                   const Camera& camera,
                                   float depth = 0.0f);
    
    static glm::vec3 GetRayDirection(glm::vec2 screen_pos,
                                    const Camera& camera);
    
    static bool RayIntersectsCube(glm::vec3 ray_origin,
                                 glm::vec3 ray_direction,
                                 glm::vec3 cube_center,
                                 float cube_size);
};
```

---

## Audio System

### Audio Manager
```cpp
class AudioManager {
public:
    AudioManager();
    ~AudioManager();
    
    void Initialize();
    void Shutdown();
    
    void PlaySound(const std::string& sound_name);
    void SetMasterVolume(float volume);
    
private:
    std::unordered_map<std::string, ALuint> sounds_;
    ALCdevice* device_;
    ALCcontext* context_;
    
    ALuint LoadSound(const std::string& filepath);
    void PlaySoundEffect(ALuint source);
};
```

### Sound Effects
- **turret_place.wav**: Electronic beep (0.5s)
- **projectile_fire.wav**: Energy zap (0.3s)
- **enemy_hit.wav**: Digital explosion (0.4s)
- **core_damage.wav**: Warning alarm (1.0s)
- **game_over.wav**: System shutdown (2.0s)

---

## Performance Optimization

### Rendering Optimization
```cpp
class RenderBatch {
public:
    void AddCube(const PositionComponent& pos, 
                 const RenderComponent& render);
    void Flush();
    
private:
    std::vector<glm::vec3> positions_;
    std::vector<glm::vec3> colors_;
    GLuint VAO_, VBO_;
    
    void CreateBuffers();
    void UpdateBuffers();
};
```

### Memory Management
```cpp
class ObjectPool {
public:
    ObjectPool(size_t initial_size);
    
    Entity GetEntity();
    void ReturnEntity(Entity entity);
    
    void Reserve(size_t count);
    void Clear();
    
private:
    std::queue<Entity> available_entities_;
    std::vector<Entity> all_entities_;
    size_t max_size_;
};
```

### Performance Monitoring
```cpp
class PerformanceProfiler {
public:
    static void BeginFrame();
    static void EndFrame();
    
    static void BeginSample(const std::string& name);
    static void EndSample();
    
    static void PrintStats();
    
private:
    struct Sample {
        std::string name;
        std::chrono::high_resolution_clock::time_point start_time;
        float duration_ms;
    };
    
    static std::vector<Sample> samples_;
    static float frame_time_;
};
```

---

## Build Configuration

### CMakeLists.txt
```cmake
cmake_minimum_required(VERSION 3.20)
project(CORE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Add executable
add_executable(CORE
    src/main.cpp
    src/core/engine.cpp
    src/graphics/renderer.cpp
    src/game/game.cpp
    # ... other source files
)

# Link libraries
target_link_libraries(CORE
    OpenGL::GL
    glfw
    glm::glm
)

# Include directories
target_include_directories(CORE PRIVATE
    src/
    external/
)
```

### Compiler Flags
```cmake
# Release flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

# Platform-specific optimizations
if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
elseif(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
endif()
```

---

## Testing Framework

### Unit Tests
```cpp
#include <gtest/gtest.h>

TEST(EntityManagerTest, CreateDestroyEntity) {
    EntityManager manager;
    Entity entity = manager.CreateEntity();
    EXPECT_NE(entity.GetID(), 0);
    
    manager.DestroyEntity(entity);
    // Verify entity is destroyed
}

TEST(CollisionSystemTest, ProjectileHitEnemy) {
    CollisionSystem collision_system;
    
    Entity projectile = CreateTestProjectile();
    Entity enemy = CreateTestEnemy();
    
    bool hit = collision_system.CheckProjectileHit(projectile, enemy);
    EXPECT_TRUE(hit);
}

TEST(SpatialGridTest, RadiusQuery) {
    SpatialGrid grid(5.0f, glm::vec3(100.0f));
    
    Entity entity = CreateEntityAt(glm::vec3(10.0f, 0.0f, 0.0f));
    grid.Insert(entity, GetPosition(entity));
    
    auto entities = grid.GetEntitiesInRadius(glm::vec3(0.0f), 15.0f);
    EXPECT_EQ(entities.size(), 1);
}
```

---

*Document Version: 1.0 - Technical Foundation*
