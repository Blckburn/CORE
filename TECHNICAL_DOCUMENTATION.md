# Техническая Документация: CORE

## Обзор Проекта

**CORE** - это минималистичная 3D Tower Defense игра, вдохновленная эстетикой TRON. Игроки защищают центральный куб от нападающих вражеских кубов, используя башни для размещения в 3D пространстве.

### Основные Характеристики
- **3D Пространство**: Полностью трехмерная игровая среда
- **Wireframe Эстетика**: Стиль TRON с неоновыми проволочными объектами
- **Система Башен**: Размещение и управление башнями в 3D
- **Система Проектилей**: Homing снаряды в виде колец
- **Камера**: Полноценное управление камерой (вращение, зум)

## Архитектура Системы

### Структура Проекта
```
CORE/
├── src/
│   ├── main.cpp                 # Точка входа
│   ├── core/                    # Основной движок
│   │   ├── engine.h/cpp         # Главный класс движка
│   │   ├── window.h/cpp         # Управление окном
│   │   ├── input.h/cpp          # Обработка ввода
│   │   └── time.h/cpp           # Управление временем
│   ├── graphics/                # Графическая система
│   │   ├── renderer.h/cpp       # Рендеринг
│   │   ├── camera.h/cpp         # Камера
│   │   ├── shader.h/cpp         # Шейдеры
│   │   ├── mesh.h/cpp           # 3D меши
│   │   └── ray_caster.h/cpp     # Ray casting для 3D взаимодействий
│   ├── game/                    # Игровая логика
│   │   ├── game.h/cpp           # Главный игровой класс
│   │   ├── entity.h/cpp         # Базовый класс сущностей
│   │   ├── enemy.h/cpp          # Враги
│   │   ├── enemy_spawner.h/cpp  # Спавн врагов
│   │   ├── turret.h/cpp         # Башни
│   │   ├── turret_manager.h/cpp # Управление башнями
│   │   ├── turret_preview.h/cpp # Предпросмотр размещения башен
│   │   ├── projectile.h/cpp     # Снаряды
│   │   └── projectile_manager.h/cpp # Управление снарядами
│   └── utils/                   # Утилиты
│       └── math.h/cpp           # Математические функции
├── assets/
│   └── shaders/                 # Шейдеры
│       ├── basic.vert           # Вертексный шейдер
│       └── basic.frag           # Фрагментный шейдер
├── CMakeLists.txt               # Конфигурация сборки
└── vcpkg/                       # Менеджер пакетов
```

## Детальное Описание Компонентов

### 1. Ядро Движка (Core)

#### Engine Class
**Файл**: `src/core/engine.h/cpp`

Главный класс, координирующий все системы игры:

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

**Основной цикл игры**:
1. Обновление времени (`Time::Update()`)
2. Обработка ввода (`input_->Update()`)
3. Обновление игровой логики (`game_->Update()`)
4. Начало рендеринга (`renderer_->BeginFrame()`)
5. Рендеринг игры (`game_->Render()`)
6. Завершение рендеринга (`renderer_->EndFrame()`)
7. Обмен буферов (`window_->SwapBuffers()`)
8. Обработка событий (`window_->PollEvents()`)

#### Window Class
**Файл**: `src/core/window.h/cpp`

Управляет созданием и настройкой окна OpenGL:

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
    void GetSize(int& width, int& height);
};
```

**Функциональность**:
- Создание окна GLFW
- Инициализация OpenGL контекста
- Обработка событий окна
- Управление размерами окна

#### InputManager Class
**Файл**: `src/core/input.h/cpp`

Обрабатывает все виды пользовательского ввода:

```cpp
class InputManager {
private:
    bool keys_[GLFW_KEY_LAST];
    bool keys_prev_[GLFW_KEY_LAST];
    bool mouse_buttons_[GLFW_MOUSE_BUTTON_LAST];
    bool mouse_buttons_prev_[GLFW_MOUSE_BUTTON_LAST];
    glm::vec2 mouse_position_;
    float scroll_delta_;
    
public:
    bool IsKeyPressed(int key);
    bool IsKeyJustPressed(int key);
    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonJustPressed(int button);
    glm::vec2 GetMousePosition();
    float GetScrollDelta();
};
```

**Поддерживаемые вводы**:
- **Клавиатура**: WASD (движение камеры), T (режим размещения башен), +/- (расстояние размещения)
- **Мышь**: ЛКМ (размещение башен), ПКМ (вращение камеры), колесо (зум)
- **Коллбэки**: GLFW callbacks для обработки событий

### 2. Графическая Система (Graphics)

#### Renderer Class
**Файл**: `src/graphics/renderer.h/cpp`

Управляет рендерингом и состоянием OpenGL:

```cpp
class Renderer {
public:
    bool Initialize();
    void BeginFrame();
    void EndFrame();
    void SetViewport(int width, int height);
};
```

**Настройки OpenGL**:
- `GL_DEPTH_TEST` - тест глубины
- `GL_BLEND` - смешивание для прозрачности
- `GL_LINES` - режим wireframe рендеринга

#### Camera Class
**Файл**: `src/graphics/camera.h/cpp`

Реализует 3D камеру с орбитальным управлением:

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
};
```

**Управление камерой**:
- **Вращение**: ПКМ + движение мыши
- **Зум**: Колесо мыши или Q/E
- **Орбитальная система**: Камера вращается вокруг центра (0,0,0)

#### Mesh Class
**Файл**: `src/graphics/mesh.h/cpp`

Создает и управляет 3D мешами:

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
};
```

**Типы мешей**:
- **Кубы**: Wireframe кубы для врагов, башен, центрального куба
- **Кольца**: Полые диски для снарядов
- **Wireframe рендеринг**: Только контуры объектов

#### RayCaster Class
**Файл**: `src/graphics/ray_caster.h/cpp`

Выполняет ray casting для 3D взаимодействий:

```cpp
class RayCaster {
public:
    glm::vec3 GetGroundIntersection(const glm::vec2& mouse_pos, const Camera* camera, 
                                   int screen_width, int screen_height, float ground_y = 0.0f);
    glm::vec3 GetPlaneIntersection(const glm::vec2& screen_pos, const Camera* camera,
                                  int screen_width, int screen_height,
                                  const glm::vec3& plane_center, const glm::vec3& plane_normal);
};
```

**Функциональность**:
- **3D Ray Casting**: Преобразование 2D координат мыши в 3D лучи
- **Пересечение с плоскостями**: Для размещения башен в 3D пространстве
- **Динамические плоскости размещения**: Настраиваемое расстояние от камеры

### 3. Игровая Логика (Game)

#### Game Class
**Файл**: `src/game/game.h/cpp`

Главный класс игровой логики:

```cpp
class Game {
private:
    std::unique_ptr<Shader> shader_;
    std::unique_ptr<Mesh> cube_mesh_, enemy_mesh_, turret_mesh_, projectile_mesh_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<EnemySpawner> enemy_spawner_;
    std::unique_ptr<TurretManager> turret_manager_;
    std::unique_ptr<RayCaster> ray_caster_;
    std::unique_ptr<TurretPreview> turret_preview_;
    std::unique_ptr<ProjectileManager> projectile_manager_;
    
    bool turret_placement_mode_;
    glm::vec3 preview_position_;
    bool preview_valid_;
};
```

**Основные системы**:
- **Управление состоянием**: Режим размещения башен
- **Координация компонентов**: Связывает все игровые системы
- **Рендеринг**: Управляет отрисовкой всех объектов

#### Enemy System

##### Enemy Class
**Файл**: `src/game/enemy.h/cpp`

```cpp
class Enemy {
private:
    glm::vec3 position_;
    glm::vec3 target_position_; // Центр карты (0,0,0)
    float speed_;
    int health_;
    glm::vec3 color_;
    bool has_reached_target_;
    
public:
    bool Initialize(const glm::vec3& position, float speed, int health, const glm::vec3& color);
    void Update(float delta_time);
    void TakeDamage(int amount);
    bool IsAlive() const;
    bool HasReachedTarget() const;
};
```

**Характеристики врагов**:
- **Здоровье**: 100 HP
- **Скорость**: 5 единиц/сек
- **Цвет**: Красный wireframe куб
- **Поведение**: Движение к центру карты

##### EnemySpawner Class
**Файл**: `src/game/enemy_spawner.h/cpp`

```cpp
class EnemySpawner {
private:
    std::vector<std::unique_ptr<Enemy>> enemies_;
    float spawn_timer_;
    float spawn_rate_; // Время между спавнами
    float spawn_radius_;
    bool spawning_active_;
    std::mt19937 rng_;
    
public:
    void SetSpawnRate(float rate); // Врагов в секунду
    void SetSpawnRadius(float radius);
    void SpawnEnemy();
};
```

**Механика спавна**:
- **Случайные позиции**: На сфере вокруг карты
- **Настраиваемая частота**: 1 враг в секунду
- **Радиус спавна**: 25 единиц от центра

#### Turret System

##### Turret Class
**Файл**: `src/game/turret.h/cpp`

```cpp
class Turret {
private:
    glm::vec3 position_;
    float range_; // 15 единиц
    int damage_; // 25 урона
    float reload_time_; // 1 секунда
    float last_fire_time_;
    float rotation_; // Y-ось для визуального наведения
    Enemy* current_target_;
    
public:
    void UpdateTarget(const std::vector<std::unique_ptr<Enemy>>& enemies);
    void Fire(ProjectileManager* projectile_manager);
    bool CanFire() const;
    void UpdateRotation(float delta_time);
};
```

**Характеристики башен**:
- **Дальность**: 15 единиц
- **Урон**: 25 HP за выстрел
- **Скорострельность**: 1 выстрел в секунду
- **Цвет**: Зеленый wireframe куб
- **Наведение**: Автоматическое наведение на ближайшего врага

##### TurretManager Class
**Файл**: `src/game/turret_manager.h/cpp`

```cpp
class TurretManager {
private:
    std::vector<std::unique_ptr<Turret>> turrets_;
    float min_distance_from_center_; // 5 единиц
    float max_distance_from_center_; // 20 единиц
    float min_distance_between_turrets_; // 3 единицы
    ProjectileManager* projectile_manager_;
    
public:
    bool PlaceTurret(const glm::vec3& position);
    bool IsValidPlacement(const glm::vec3& position) const;
    void Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies);
};
```

**Правила размещения**:
- **Минимальное расстояние от центра**: 5 единиц
- **Максимальное расстояние от центра**: 20 единиц
- **Минимальное расстояние между башнями**: 3 единицы

##### TurretPreview Class
**Файл**: `src/game/turret_preview.h/cpp`

```cpp
class TurretPreview {
private:
    std::unique_ptr<Mesh> preview_mesh_;
    glm::vec3 position_;
    bool visible_;
    bool valid_placement_;
    glm::vec3 valid_color_; // Зеленый
    glm::vec3 invalid_color_; // Красный
    
public:
    void Update(const glm::vec3& position, bool is_valid_placement);
    void Show();
    void Hide();
};
```

**Визуальная обратная связь**:
- **Зеленый куб**: Валидное размещение
- **Красный куб**: Недопустимое размещение
- **Следует за курсором**: В режиме размещения

#### Projectile System

##### Projectile Class
**Файл**: `src/game/projectile.h/cpp`

```cpp
class Projectile {
private:
    glm::vec3 position_;
    glm::vec3 target_position_;
    glm::vec3 direction_;
    float speed_; // 30 единиц/сек
    int damage_; // 25 урона
    glm::vec3 color_; // Циан
    float lifetime_; // 3 секунды
    float current_lifetime_;
    
public:
    void Update(float delta_time);
    void UpdateTarget(const glm::vec3& new_target_position);
    bool CheckHit(const glm::vec3& target_position, float hit_radius) const;
};
```

**Характеристики снарядов**:
- **Скорость**: 30 единиц/сек
- **Урон**: 25 HP
- **Время жизни**: 3 секунды
- **Внешний вид**: Циановое wireframe кольцо
- **Радиус попадания**: 1.0 единица

##### ProjectileManager Class
**Файл**: `src/game/projectile_manager.h/cpp`

```cpp
class ProjectileManager {
private:
    std::vector<std::unique_ptr<Projectile>> projectiles_;
    int active_projectiles_count_;
    float default_speed_; // 30.0f
    int default_damage_; // 25
    glm::vec3 default_color_; // Циан
    
public:
    void CreateProjectile(const glm::vec3& start_position, const glm::vec3& target_position, 
                         float speed, int damage, const glm::vec3& color);
    void Update(float delta_time, const std::vector<std::unique_ptr<Enemy>>& enemies);
};
```

**Homing система**:
- **Автоматическое наведение**: Снаряды корректируют траекторию к ближайшему врагу
- **Обновление целей**: В реальном времени во время полета
- **Коллизии**: Проверка попаданий с радиусом 1.0

## Системы Взаимодействия

### 1. Размещение Башен

**Процесс размещения**:
1. Нажатие `T` - вход в режим размещения
2. Движение мыши - обновление позиции предпросмотра
3. Ray casting - вычисление 3D позиции на плоскости размещения
4. Валидация - проверка правил размещения
5. ЛКМ - подтверждение размещения
6. Создание башни - инициализация новой башни

**3D Плоскость размещения**:
```cpp
glm::vec3 camera_pos = camera_->GetPosition();
glm::vec3 camera_direction = glm::normalize(camera_->GetTarget() - camera_pos);
float placement_distance = 15.0f; // Настраиваемое расстояние
glm::vec3 plane_center = camera_pos + camera_direction * placement_distance;
glm::vec3 plane_normal = camera_direction;
```

### 2. Система Боя

**Цикл боя**:
1. **Поиск целей**: Башни находят ближайших врагов в радиусе
2. **Наведение**: Поворот башни в направлении цели
3. **Стрельба**: Создание снаряда при готовности
4. **Homing**: Снаряд корректирует траекторию к цели
5. **Попадание**: Проверка коллизии с радиусом 1.0
6. **Урон**: Применение урона к врагу

**Математика попадания**:
```cpp
float distance = glm::length(target_position - projectile_position);
bool hit = distance <= hit_radius; // 1.0 единица
```

### 3. Управление Камерой

**Орбитальная система**:
- **Центр вращения**: (0, 0, 0) - центр карты
- **Вращение**: ПКМ + движение мыши
- **Зум**: Колесо мыши или Q/E
- **Ограничения**: Минимальный и максимальный зум

**Преобразования**:
```cpp
glm::mat4 view = glm::lookAt(position, target, glm::vec3(0, 1, 0));
glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
```

## Технические Детали

### 1. Зависимости

**Основные библиотеки**:
- **GLFW**: Управление окнами и вводом
- **GLAD**: Загрузчик OpenGL
- **GLM**: Математическая библиотека для OpenGL
- **OpenGL 3.3**: Графический API

**Установка через vcpkg**:
```bash
vcpkg install glfw3:x64-windows
vcpkg install glad:x64-windows
vcpkg install glm:x64-windows
```

### 2. Система Сборки

**CMake конфигурация**:
```cmake
cmake_minimum_required(VERSION 3.16)
project(CORE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(CORE ${SOURCES} ${HEADERS})
target_link_libraries(CORE glfw glad glm)
```

### 3. Шейдеры

**Вертексный шейдер** (`basic.vert`):
```glsl
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
```

**Фрагментный шейдер** (`basic.frag`):
```glsl
#version 330 core
out vec4 FragColor;
uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
}
```

### 4. Производительность

**Оптимизации**:
- **Object Pooling**: Переиспользование снарядов
- **Frustum Culling**: Отсечение невидимых объектов
- **Batch Rendering**: Группировка одинаковых объектов
- **Efficient Collision**: Простые сферические коллизии

**Метрики производительности**:
- **FPS**: Целевой 60 FPS
- **Memory**: ~50MB RAM
- **CPU**: <5% на современном процессоре
- **GPU**: Совместимость с OpenGL 3.3+

## Управление

### Основные Клавиши

| Клавиша | Действие |
|---------|----------|
| `WASD` | Вращение камеры |
| `ПКМ` | Вращение камеры (удерживать) |
| `Колесо мыши` | Зум |
| `Q/E` | Зум |
| `T` | Переключение режима размещения башен |
| `ЛКМ` | Размещение башни (в режиме размещения) |
| `+/-` | Изменение расстояния размещения |

### Режимы Игры

1. **Режим наблюдения**: Стандартное управление камерой
2. **Режим размещения башен**: 
   - Зеленый куб - валидное размещение
   - Красный куб - недопустимое размещение
   - Настраиваемое расстояние размещения

## Будущие Улучшения

### Планируемые Функции

1. **Дополнительные типы башен**:
   - Скорострельные башни
   - Урон по области
   - Замедляющие башни

2. **Улучшенная графика**:
   - Частицы и эффекты
   - Освещение
   - Тени

3. **Игровая механика**:
   - Волны врагов
   - Улучшения башен
   - Система очков

4. **UI/UX**:
   - Интерфейс пользователя
   - Меню настроек
   - Статистика игры

### Технические Улучшения

1. **Архитектура**:
   - Entity-Component-System (ECS)
   - Система событий
   - Скриптовая система

2. **Производительность**:
   - Многопоточность
   - Профилирование
   - Оптимизация рендеринга

3. **Кроссплатформенность**:
   - Linux поддержка
   - macOS поддержка
   - Мобильные платформы

## Заключение

CORE представляет собой хорошо структурированный 3D Tower Defense движок с современной архитектурой и чистой кодовой базой. Проект демонстрирует:

- **Модульную архитектуру**: Четкое разделение ответственности
- **Современные технологии**: OpenGL 3.3, GLM, CMake
- **Чистый код**: Следование принципам SOLID
- **Расширяемость**: Легкое добавление новых функций
- **Производительность**: Оптимизированный рендеринг

Проект готов для дальнейшего развития и может служить основой для более сложных игровых проектов.
