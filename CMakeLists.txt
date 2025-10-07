cmake_minimum_required(VERSION 3.20)
project(CORE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
endif()

# Find required packages using vcpkg
find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Freetype REQUIRED)

# Include directories
include_directories(src)
include_directories(external)

# Source files
set(SOURCES
    src/main.cpp
    src/core/engine.cpp
    src/core/window.cpp
    src/core/input.cpp
    src/core/time.cpp
    src/graphics/renderer.cpp
    src/graphics/shader.cpp
    src/graphics/mesh.cpp
    src/graphics/camera.cpp
    src/graphics/ray_caster.cpp
    src/graphics/font.cpp
    src/game/game.cpp
    src/game/entity.cpp
    src/game/enemy.cpp
    src/game/enemy_spawner.cpp
    src/game/turret.cpp
    src/game/turret_manager.cpp
    src/game/projectile.cpp
    src/game/projectile_manager.cpp
    src/game/turret_preview.cpp
    src/game/wave_manager.cpp
    src/game/ui_manager.cpp
    src/game/item.cpp
    src/game/item_manager.cpp
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
    src/graphics/shader.h
    src/graphics/mesh.h
    src/graphics/camera.h
    src/graphics/ray_caster.h
    src/graphics/font.h
    src/game/game.h
    src/game/entity.h
    src/game/enemy.h
    src/game/enemy_spawner.h
    src/game/turret.h
    src/game/turret_manager.h
    src/game/projectile.h
    src/game/projectile_manager.h
    src/game/turret_preview.h
    src/game/wave_manager.h
    src/game/ui_manager.h
    src/utils/math.h
    src/utils/debug.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    glm::glm
    glad::glad
    Freetype::Freetype
)

# Copy assets to build directory
file(COPY assets/shaders DESTINATION ${CMAKE_BINARY_DIR}/assets/)
file(COPY assets/fonts DESTINATION ${CMAKE_BINARY_DIR}/assets/)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "OpenGL found: ${OpenGL_FOUND}")
message(STATUS "GLFW3 found: ${glfw3_FOUND}")
message(STATUS "GLM found: ${glm_FOUND}")
message(STATUS "GLAD found: ${glad_FOUND}")
