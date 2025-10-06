# Техническая Архитектурная Спецификация: CORE

## Обзор Проекта

**CORE** — это минималистичная 3D Tower Defense игра, построенная на собственном движке C++. Проект подчеркивает чистую архитектуру, современные практики C++ и оптимизацию производительности, сохраняя при этом киберпанк wireframe эстетику и механики 3D геймплея.

## Архитектура Движка

### Основные Принципы Дизайна

1. **Модульность**: Четкое разделение ответственности между системами
2. **Производительность**: Оптимизация для 60 FPS на целевом оборудовании
3. **Расширяемость**: Легкое добавление новых функций
4. **Поддерживаемость**: Чистая, читаемая структура кода
5. **Современный C++**: Использование функций C++17 и лучших практик

### Архитектура Системы

```
CORE Engine
├── Основные Системы
│   ├── Engine (Главный координатор)
│   ├── Window Management (GLFW)
│   ├── Input Management
│   ├── Time Management
│   └── Event System
├── Графический Конвейер
│   ├── Renderer (OpenGL 3.3)
│   ├── Camera System
│   ├── Shader Management
│   ├── Mesh System
│   └── Ray Casting
├── Игровая Логика
│   ├── Game State Management
│   ├── Entity System
│   ├── Enemy System
│   ├── Turret System
│   └── Projectile System
└── Утилиты
    ├── Mathematics (GLM)
    ├── Debug Tools
    └── Performance Profiling
```

## Детальное Описание Систем

### 1. Основные Системы

#### Engine Class
**Ответственность**: Координация основного игрового цикла и инициализация систем

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

**Ключевые Особенности**:
- Управление порядком инициализации систем
- Координация основного игрового цикла
- Процедуры чистого завершения
- Обработка ошибок и восстановление

#### Window Management
**Ответственность**: Создание окна GLFW и настройка контекста OpenGL

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

**Особенности**:
- Создание контекста OpenGL 3.3
- Обработка событий окна
- Обработка изменения размера
- Готовность к поддержке нескольких мониторов

#### Input Management
**Ответственность**: Централизованная обработка ввода с отслеживанием состояния

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

**Особенности**:
- Отслеживание состояния ввода с точностью до кадра
- Ввод мыши и клавиатуры
- Поддержка колеса прокрутки
- Настраиваемые привязки клавиш

#### Time Management
**Ответственность**: Время кадров и расчет delta time

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

**Особенности**:
- Точный расчет delta time
- Мониторинг FPS
- Ограничение частоты кадров
- Метрики производительности

### 2. Графический Конвейер

#### Renderer System
**Ответственность**: Управление состоянием OpenGL и координация рендеринга

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

**Конфигурация OpenGL**:
- Включено тестирование глубины
- Смешивание для прозрачности
- Режим wireframe рендеринга
- Оптимизированные изменения состояния

#### Camera System
**Ответственность**: 3D камера с орбитальным управлением

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

**Особенности**:
- Орбитальное управление камерой
- Плавное вращение и зум
- Настраиваемое поле зрения
- Перспективная проекция

#### Shader System
**Ответственность**: Компиляция и управление шейдерами OpenGL

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

**Конвейер Шейдеров**:
- Вертексный шейдер: 3D трансформация
- Фрагментный шейдер: Вывод цвета
- Управление uniform переменными
- Обработка ошибок и валидация

#### Mesh System
**Ответственность**: Создание и рендеринг 3D геометрии

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

**Типы Мешей**:
- Wireframe кубы для игровых объектов
- Полые кольца для проектилей
- Эффективное управление vertex buffer
- Рендеринг на основе индексов

#### Ray Casting System
**Ответственность**: 3D взаимодействие и обнаружение столкновений

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

**Особенности**:
- Преобразование экранных координат в мировые
- Пересечение 3D плоскостей
- Тестирование пересечения сфер
- Эффективные алгоритмы ray casting

### 3. Системы Игровой Логики

#### Game State Management
**Ответственность**: Общая координация состояния игры

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

**Особенности**:
- Координация компонентов
- Управление состоянием
- Координация рендеринга
- Делегирование обработки ввода

#### Entity System
**Ответственность**: Базовая функциональность сущностей

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

**Особенности**:
- Базовый интерфейс сущности
- Управление трансформациями
- Управление жизненным циклом
- Расширяемый дизайн

#### Enemy System
**Ответственность**: Спавн, движение и жизненный цикл врагов

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

**Особенности**:
- Случайное позиционирование спавна
- Настраиваемые скорости спавна
- Эффективное управление врагами
- Обнаружение столкновений

#### Turret System
**Ответственность**: Размещение, наведение и стрельба турелей

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

**Особенности**:
- Валидация 3D размещения
- Автоматическое наведение
- Настраиваемые правила размещения
- Эффективное управление турелями

#### Projectile System
**Ответственность**: Создание, движение и столкновение проектилей

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

**Особенности**:
- Поведение самонаводящихся проектилей
- Эффективное обнаружение столкновений
- Управление временем жизни
- Оптимизация производительности

### 4. Системы Утилит

#### Mathematics
**Ответственность**: Математические операции и трансформации

```cpp
namespace Math {
    glm::mat4 CreateTransformMatrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
    glm::vec3 ScreenToWorldRay(const glm::vec2& screen_pos, const Camera* camera, int screen_width, int screen_height);
    bool RaySphereIntersection(const glm::vec3& ray_origin, const glm::vec3& ray_direction, 
                              const glm::vec3& sphere_center, float sphere_radius, float& t);
}
```

**Особенности**:
- Интеграция GLM
- Пользовательские математические утилиты
- Помощники 3D трансформации
- Алгоритмы пересечения

#### Debug System
**Ответственность**: Вывод отладки и мониторинг производительности

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

**Особенности**:
- Управление выводом консоли
- Метрики производительности
- Визуальный отладочный рендеринг
- Настраиваемые уровни отладки

## Система Сборки

### CMake Конфигурация

```cmake
cmake_minimum_required(VERSION 3.16)
project(CORE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Интеграция vcpkg
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

# Поиск пакетов
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Исходные файлы
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

# Заголовочные файлы
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

# Создание исполняемого файла
add_executable(CORE ${SOURCES} ${HEADERS})

# Связывание библиотек
target_link_libraries(CORE glfw glad glm)

# Опции компилятора
if(MSVC)
    target_compile_options(CORE PRIVATE /W4)
else()
    target_compile_options(CORE PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

### Зависимости

**Основные Библиотеки**:
- **GLFW 3.3+**: Управление окнами и вводом
- **GLAD**: Загрузчик OpenGL
- **GLM 0.9.9+**: Библиотека математики
- **OpenGL 3.3**: API графики

**Инструменты Разработки**:
- **CMake 3.16+**: Система сборки
- **vcpkg**: Менеджер пакетов
- **Visual Studio 2019+**: IDE (Windows)
- **Git**: Контроль версий

## Соображения Производительности

### Стратегии Оптимизации

1. **Object Pooling**: Переиспользование объектов проектилей для уменьшения выделений
2. **Batch Rendering**: Группировка похожих объектов для эффективных вызовов отрисовки
3. **Frustum Culling**: Пропуск рендеринга объектов вне экрана
4. **Efficient Collision**: Использование простого сферического обнаружения столкновений
5. **Memory Management**: Принципы умных указателей и RAII

### Цели Производительности

- **Частота Кадров**: 60 FPS на целевом оборудовании
- **Использование Памяти**: <100MB RAM
- **Время Загрузки**: <3 секунд запуска
- **Использование CPU**: <10% на современных процессорах
- **Использование GPU**: Оптимизация для встроенной графики

### Инструменты Профилирования

- **Встроенный Счетчик FPS**: Мониторинг производительности в реальном времени
- **Отслеживание Памяти**: Мониторинг выделений
- **Анализ Времени Кадра**: Разбивка производительности по кадрам
- **Профилирование GPU**: Анализ производительности OpenGL

## Обработка Ошибок

### Стратегия Управления Ошибками

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

**Особенности Обработки Ошибок**:
- Комплексные коды ошибок
- Проверка ошибок OpenGL
- Изящное восстановление от ошибок
- Подробное логирование ошибок

## Будущие Улучшения Архитектуры

### Планируемые Улучшения

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

2. **Система Событий**:
   ```cpp
   class EventBus {
   public:
       template<typename T> void Subscribe(std::function<void(const T&)> callback);
       template<typename T> void Publish(const T& event);
   };
   ```

3. **Интеграция Скриптинга**:
   - Lua скриптинг для игровой логики
   - Python инструменты для создания контента
   - Возможности горячей перезагрузки

4. **Продвинутый Рендеринг**:
   - Конвейер отложенного затенения
   - Карты теней
   - Пост-обработка эффектов
   - Системы частиц

5. **Аудио Система**:
   - 3D позиционное аудио
   - Динамическая музыкальная система
   - Управление звуковыми эффектами

### Соображения Масштабируемости

- **Модульный Дизайн**: Легкое добавление новых систем
- **Архитектура Плагинов**: Расширяемая функциональность
- **Система Конфигурации**: Настройка параметров во время выполнения
- **Управление Ресурсами**: Эффективная загрузка и кэширование ресурсов

## Заключение

Архитектура движка CORE обеспечивает прочную основу для 3D Tower Defense геймплея, сохраняя при этом чистый, поддерживаемый код. Модульный дизайн позволяет легко расширять и модифицировать, в то время как оптимизации производительности обеспечивают плавный геймплей на целевом оборудовании.

Ключевые архитектурные преимущества:
- **Четкое разделение ответственности**
- **Современные практики C++**
- **Оптимизация производительности**
- **Расширяемый дизайн**
- **Комплексная обработка ошибок**

Архитектура спроектирована для роста вместе с проектом, поддерживая будущие улучшения при сохранении основных принципов простоты, производительности и поддерживаемости.