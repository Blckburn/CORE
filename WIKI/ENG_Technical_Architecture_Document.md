# Technical Architecture Specification: CORE

## Project Overview

**CORE** is a minimalist 3D Tower Defense game built with a custom C++ engine. The project emphasizes clean architecture, modern C++ practices, and performance optimization while maintaining the TRON-inspired aesthetic and 3D gameplay mechanics.

## Engine Architecture

### Core Design Principles

1. **Modularity**: Clear separation of concerns between systems
2. **Performance**: Optimized for 60 FPS on target hardware
3. **Extensibility**: Easy addition of new features
4. **Maintainability**: Clean, readable code structure
5. **Modern C++**: Leveraging C++17 features and best practices

### System Architecture

```
CORE Engine
├── Core Systems
│   ├── Engine (Main coordinator)
│   ├── Window Management (GLFW)
│   ├── Input Management
│   ├── Time Management
│   └── Event System
├── Graphics Pipeline
│   ├── Renderer (OpenGL 3.3)
│   ├── Camera System
│   ├── Shader Management
│   ├── Mesh System
│   └── Ray Casting
├── Game Logic
│   ├── Game State Management
│   ├── Entity System
│   ├── Enemy System
│   ├── Turret System
│   └── Projectile System
└── Utilities
    ├── Mathematics (GLM)
    ├── Debug Tools
    └── Performance Profiling
```

## Detailed System Description

### 1. Core Systems

#### Engine Class
**Responsibility**: Main game loop coordination and system initialization

```cpp
class Engine {
private:
    std::unique_ptr<Window> window_;
    std::unique_ptr<InputManager> input_;
    std::unique_ptr<Renderer> renderer_;
    std::unique_ptr<Game> game_;
    
public:
    bool Initialize();
    void Run();
    void Shutdown();
};
```

**Key Features**:
- System initialization order management
- Main game loop coordination
- Clean shutdown procedures
- Error handling and recovery

#### Window Management
**Responsibility**: GLFW window creation and OpenGL context setup

```cpp
class Window {
private:
    GLFWwindow* window_;
    int width_, height_;
    
public:
    bool Initialize(const char* title, int width, int height);
    void SwapBuffers();
    void PollEvents();
    bool ShouldClose();
};
```

**Features**:
- OpenGL 3.3 context creation
- Window event handling
- Resize handling
- Multi-monitor support ready

#### Input Management
**Responsibility**: Centralized input handling with state tracking

```cpp
class InputManager {
private:
    bool keys_[GLFW_KEY_LAST];
    bool keys_prev_[GLFW_KEY_LAST];
    bool mouse_buttons_[GLFW_MOUSE_BUTTON_LAST];
    glm::vec2 mouse_position_;
    float scroll_delta_;
    
public:
    bool IsKeyPressed(int key);
    bool IsKeyJustPressed(int key);
    glm::vec2 GetMousePosition();
    float GetScrollDelta();
};
```

**Features**:
- Frame-accurate input state tracking
- Mouse and keyboard input
- Scroll wheel support
- Configurable key bindings

#### Time Management
**Responsibility**: Frame timing and delta time calculation

```cpp
class Time {
private:
    static float delta_time_;
    static float last_frame_time_;
    
public:
    static void Initialize();
    static void Update();
    static float GetDeltaTime();
    static float GetFPS();
};
```

**Features**:
- Precise delta time calculation
- FPS monitoring
- Frame rate limiting
- Performance metrics

### 2. Graphics Pipeline

#### Renderer System
**Responsibility**: OpenGL state management and rendering coordination

```cpp
class Renderer {
public:
    bool Initialize();
    void BeginFrame();
    void EndFrame();
    void SetViewport(int width, int height);
    void SetClearColor(float r, float g, float b, float a);
};
```

**OpenGL Configuration**:
- Depth testing enabled
- Blending for transparency
- Wireframe rendering mode
- Optimized state changes

#### Camera System
**Responsibility**: 3D camera with orbital control

```cpp
class Camera {
private:
    glm::vec3 position_;
    glm::vec3 target_;
    float zoom_;
    float rotation_x_, rotation_y_;
    
public:
    void SetZoom(float zoom);
    void Rotate(float delta_x, float delta_y);
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix();
    glm::vec3 GetPosition();
};
```

**Features**:
- Orbital camera control
- Smooth rotation and zoom
- Configurable field of view
- Perspective projection

#### Shader System
**Responsibility**: OpenGL shader compilation and management

```cpp
class Shader {
private:
    GLuint program_id_;
    
public:
    bool LoadFromFiles(const std::string& vertex_path, const std::string& fragment_path);
    void Use();
    void SetUniform(const std::string& name, const glm::mat4& matrix);
    void SetUniform(const std::string& name, const glm::vec3& vector);
    void SetUniform(const std::string& name, float value);
};
```

**Shader Pipeline**:
- Vertex shader: 3D transformation
- Fragment shader: Color output
- Uniform management
- Error handling and validation

#### Mesh System
**Responsibility**: 3D geometry creation and rendering

```cpp
class Mesh {
private:
    GLuint VAO_, VBO_, EBO_;
    size_t index_count_;
    bool created_;
    
public:
    bool Create();
    void Render();
    void RenderWireframe();
    void CreateCube();
    void CreateCubeWireframe();
    void CreateDisc(float radius, int segments);
    void Destroy();
};
```

**Mesh Types**:
- Wireframe cubes for game objects
- Hollow rings for projectiles
- Efficient vertex buffer management
- Index-based rendering

#### Ray Casting System
**Responsibility**: 3D interaction and collision detection

```cpp
class RayCaster {
public:
    glm::vec3 GetGroundIntersection(const glm::vec2& mouse_pos, const Camera* camera, 
                                   int screen_width, int screen_height, float ground_y = 0.0f);
    glm::vec3 GetPlaneIntersection(const glm::vec2& screen_pos, const Camera* camera,
                                  int screen_width, int screen_height,
                                  const glm::vec3& plane_center, const glm::vec3& plane_normal);
    glm::vec3 GetSphereIntersection(const glm::vec2& screen_pos, const Camera* camera,
                                   int screen_width, int screen_height,
                                   const glm::vec3& sphere_center, float sphere_radius);
};
```

**Features**:
- Screen-to-world coordinate conversion
- 3D plane intersection
- Sphere intersection testing
- Efficient ray casting algorithms

### 3. Game Logic Systems

#### Game State Management
**Responsibility**: Overall game state coordination

```cpp
class Game {
private:
    std::unique_ptr<Shader> shader_;
    std::unique_ptr<Mesh> cube_mesh_, enemy_mesh_, turret_mesh_, projectile_mesh_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<EnemySpawner> enemy_spawner_;
    std::unique_ptr<TurretManager> turret_manager_;
    std::unique_ptr<ProjectileManager> projectile_manager_;
    
    bool turret_placement_mode_;
    glm::vec3 preview_position_;
    bool preview_valid_;
};
```

**Features**:
- Component coordination
- State management
- Rendering coordination
- Input handling delegation

#### Entity System
**Responsibility**: Base entity functionality

```cpp
class Entity {
protected:
    glm::vec3 position_;
    glm::vec3 rotation_;
    glm::vec3 scale_;
    bool active_;
    
public:
    virtual void Update(float delta_time) = 0;
    virtual void Render() = 0;
    virtual bool Initialize() = 0;
};
```

**Features**:
- Base entity interface
- Transform management
- Lifecycle management
- Extensible design

#### Enemy System
**Responsibility**: Enemy spawning, movement, and lifecycle

```cpp
class Enemy : public Entity {
private:
    glm::vec3 target_position_;
    float speed_;
    int health_;
    glm::vec3 color_;
    bool has_reached_target_;
    
public:
    void Update(float delta_time) override;
    void TakeDamage(int amount);
    bool IsAlive() const;
    bool HasReachedTarget() const;
};

class EnemySpawner {
private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
    float spawn_timer_;
    float spawn_rate_;
    float spawn_radius_;
    std::mt19937 rng_;
    
public:
    void SetSpawnRate(float rate);
    void SetSpawnRadius(float radius);
    void SpawnEnemy();
    void Update(float delta_time);
};
```

**Features**:
- Random spawn positioning
- Configurable spawn rates
- Efficient enemy management
- Collision detection

#### Turret System
**Responsibility**: Turret placement, targeting, and firing

```cpp
class Turret : public Entity {
private:
    float range_;
    int damage_;
    float reload_time_;
    float last_fire_time_;
    float rotation_;
    Enemy* current_target_;
    
public:
    void Update(float delta_time) override;
    void UpdateTarget(const std::vector<std::unique_ptr<Enemy>>& enemies);
    void Fire(ProjectileManager* projectile_manager);
    bool CanFire() const;
};

class TurretManager {
private:
    std::vector<std::unique_ptr<Turret>> turrets_;
    float min_distance_from_center_;
    float max_distance_from_center_;
    float min_distance_between_turrets_;
    
public:
    bool PlaceTurret(const glm::vec3& position);
    bool IsValidPlacement(const glm::vec3& position) const;
    void Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies);
};
```

**Features**:
- 3D placement validation
- Automatic targeting
- Configurable placement rules
- Efficient turret management

#### Projectile System
**Responsibility**: Projectile creation, movement, and collision

```cpp
class Projectile : public Entity {
private:
    glm::vec3 target_position_;
    glm::vec3 direction_;
    float speed_;
    int damage_;
    float lifetime_;
    float current_lifetime_;
    
public:
    void Update(float delta_time) override;
    void UpdateTarget(const glm::vec3& new_target_position);
    bool CheckHit(const glm::vec3& target_position, float hit_radius) const;
};

class ProjectileManager {
private:
    std::vector<std::unique_ptr<Projectile>> projectiles_;
    int active_projectiles_count_;
    
public:
    void CreateProjectile(const glm::vec3& start_position, const glm::vec3& target_position, 
                         float speed, int damage);
    void Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies);
};
```

**Features**:
- Homing projectile behavior
- Efficient collision detection
- Lifetime management
- Performance optimization

### 4. Utility Systems

#### Mathematics
**Responsibility**: Mathematical operations and transformations

```cpp
namespace Math {
    glm::mat4 CreateTransformMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    glm::vec3 ScreenToWorldRay(const glm::vec2& screen_pos, const Camera* camera, int screen_width, int screen_height);
    bool RaySphereIntersection(const glm::vec3& ray_origin, const glm::vec3& ray_direction, 
                              const glm::vec3& sphere_center, float sphere_radius, float& t);
}
```

**Features**:
- GLM integration
- Custom mathematical utilities
- 3D transformation helpers
- Intersection algorithms

#### Debug System
**Responsibility**: Debug output and performance monitoring

```cpp
class Debug {
public:
    static void Log(const std::string& message);
    static void LogError(const std::string& message);
    static void LogWarning(const std::string& message);
    static void DrawDebugLine(const glm::vec3& start, const glm::vec3& end, const glm::vec3& color);
    static void DrawDebugSphere(const glm::vec3& center, float radius, const glm::vec3& color);
};
```

**Features**:
- Console output management
- Performance metrics
- Visual debug rendering
- Configurable debug levels

## Build System

### CMake Configuration

```cmake
cmake_minimum_required(VERSION 3.16)
project(CORE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg integration
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

# Find packages
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Source files
set(SOURCES
    src/main.cpp
    src/core/engine.cpp
    src/core/window.cpp
    src/core/input.cpp
    src/core/time.cpp
    src/graphics/renderer.cpp
    src/graphics/camera.cpp
    src/graphics/shader.cpp
    src/graphics/mesh.cpp
    src/graphics/ray_caster.cpp
    src/game/game.cpp
    src/game/entity.cpp
    src/game/enemy.cpp
    src/game/enemy_spawner.cpp
    src/game/turret.cpp
    src/game/turret_manager.cpp
    src/game/turret_preview.cpp
    src/game/projectile.cpp
    src/game/projectile_manager.cpp
    src/utils/math.cpp
    src/utils/debug.cpp
)

# Header files
set(HEADERS
    src/core/engine.h
    src/core/window.h
    src/core/input.h
    src/core/time.h
    src/graphics/renderer.h
    src/graphics/camera.h
    src/graphics/shader.h
    src/graphics/mesh.h
    src/graphics/ray_caster.h
    src/game/game.h
    src/game/entity.h
    src/game/enemy.h
    src/game/enemy_spawner.h
    src/game/turret.h
    src/game/turret_manager.h
    src/game/turret_preview.h
    src/game/projectile.h
    src/game/projectile_manager.h
    src/utils/math.h
    src/utils/debug.h
)

# Create executable
add_executable(CORE ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(CORE glfw glad glm)

# Compiler-specific options
if(MSVC)
    target_compile_options(CORE PRIVATE /W4)
else()
    target_compile_options(CORE PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

### Dependencies

**Core Libraries**:
- **GLFW 3.3+**: Window and input management
- **GLAD**: OpenGL loader
- **GLM 0.9.9+**: Mathematics library
- **OpenGL 3.3**: Graphics API

**Development Tools**:
- **CMake 3.16+**: Build system
- **vcpkg**: Package manager
- **Visual Studio 2019+**: IDE (Windows)
- **Git**: Version control

## Performance Considerations

### Optimization Strategies

1. **Object Pooling**: Reuse projectile objects to reduce allocations
2. **Batch Rendering**: Group similar objects for efficient draw calls
3. **Frustum Culling**: Skip rendering of off-screen objects
4. **Efficient Collision**: Use simple spherical collision detection
5. **Memory Management**: Smart pointers and RAII principles

### Performance Targets

- **Frame Rate**: 60 FPS on target hardware
- **Memory Usage**: <100MB RAM
- **Load Time**: <3 seconds startup
- **CPU Usage**: <10% on modern processors
- **GPU Usage**: Optimized for integrated graphics

### Profiling Tools

- **Built-in FPS Counter**: Real-time performance monitoring
- **Memory Tracking**: Allocation monitoring
- **Frame Time Analysis**: Per-frame performance breakdown
- **GPU Profiling**: OpenGL performance analysis

## Error Handling

### Error Management Strategy

```cpp
enum class ErrorCode {
    SUCCESS = 0,
    WINDOW_CREATION_FAILED,
    OPENGL_INITIALIZATION_FAILED,
    SHADER_COMPILATION_FAILED,
    MESH_CREATION_FAILED,
    FILE_NOT_FOUND,
    INVALID_PARAMETER
};

class Error {
public:
    static void Log(ErrorCode code, const std::string& message);
    static bool CheckGLError();
    static void HandleCriticalError(ErrorCode code, const std::string& message);
};
```

**Error Handling Features**:
- Comprehensive error codes
- OpenGL error checking
- Graceful error recovery
- Detailed error logging

## Future Architecture Enhancements

### Planned Improvements

1. **Entity-Component-System (ECS)**:
   ```cpp
   class EntityManager {
   public:
       Entity CreateEntity();
       void DestroyEntity(Entity entity);
       template<typename T> T& AddComponent(Entity entity);
       template<typename T> T& GetComponent(Entity entity);
   };
   ```

2. **Event System**:
   ```cpp
   class EventBus {
   public:
       template<typename T> void Subscribe(std::function<void(const T&)> callback);
       template<typename T> void Publish(const T& event);
   };
   ```

3. **Scripting Integration**:
   - Lua scripting for game logic
   - Python tools for content creation
   - Hot-reloading capabilities

4. **Advanced Rendering**:
   - Deferred shading pipeline
   - Shadow mapping
   - Post-processing effects
   - Particle systems

5. **Audio System**:
   - 3D positional audio
   - Dynamic music system
   - Sound effect management

### Scalability Considerations

- **Modular Design**: Easy addition of new systems
- **Plugin Architecture**: Extensible functionality
- **Configuration System**: Runtime parameter adjustment
- **Resource Management**: Efficient asset loading and caching

## Conclusion

The CORE engine architecture provides a solid foundation for 3D Tower Defense gameplay while maintaining clean, maintainable code. The modular design allows for easy extension and modification, while the performance optimizations ensure smooth gameplay on target hardware.

Key architectural strengths:
- **Clear separation of concerns**
- **Modern C++ practices**
- **Performance optimization**
- **Extensible design**
- **Comprehensive error handling**

The architecture is designed to grow with the project, supporting future enhancements while maintaining the core principles of simplicity, performance, and maintainability.
