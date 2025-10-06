# Game Design Document: CORE
## Minimalist 3D Tower Defense

---

## Core Concept

CORE is a minimalist 3D tower defense game where players defend a central cube from incoming enemy cubes that attack from all three-dimensional planes. The game features a cyberpunk aesthetic inspired by the TRON universe, with neon glows, geometric shapes, and a pure black void environment. Players strategically place defensive cube turrets in 3D space to intercept and destroy enemies before they reach the central core.

## Gameplay Mechanics

### Core Defense Objective
- **Central Cube**: Positioned at world origin (0, 0, 0)
- **Health**: 100 HP (configurable)
- **Damage on Contact**: 1 HP per enemy cube collision
- **Visual**: Neon cyan cube with glow effect

### Enemy System
- **Spawn Location**: Random position on sphere radius 50 units from center
- **Movement**: Linear path toward center cube at 5 units/second
- **Health**: 1 HP (Level 1)
- **Damage to Core**: 1 HP on contact
- **Spawn Rate**: 1 enemy every 2 seconds
- **Visual**: Neon red cube with glow effect
- **Behavior**: Direct pathfinding to center, no avoidance

### Tower System
- **Placement**: Click to place in 3D space (free-form positioning)
- **Type**: Basic Cube Turret (Level 1)
- **Damage**: 1 damage per projectile
- **Range**: 15 units radius
- **Fire Rate**: 1 projectile per second
- **Projectile Speed**: 20 units/second
- **Cost**: 10 energy points per turret
- **Visual**: Neon yellow cube with glow effect
- **Targeting**: Nearest enemy within range

### Projectile System
- **Type**: Energy projectile
- **Damage**: 1 HP per hit
- **Speed**: 20 units/second
- **Visual**: Bright white line with trail effect
- **Duration**: 2 seconds maximum flight time
- **Collision**: Instant hit detection

### Resource System
- **Energy Points**: Starting amount 50
- **Generation**: +1 energy per second
- **Turret Cost**: 10 energy per turret
- **Maximum**: No cap on energy accumulation

### Wave System
- **Type**: Continuous spawning (no waves)
- **Escalation**: Every 60 seconds, spawn rate increases by 0.1 enemies/second
- **Maximum Rate**: 5 enemies/second cap
- **Enemy Health Scaling**: +1 HP every 120 seconds

## Visual Style

### Color Palette
- **Background**: Pure black (#000000)
- **Player Core Cube**: Neon cyan (#00FFFF) with 50% opacity glow
- **Enemy Cubes**: Neon red (#FF0040) with 30% opacity glow
- **Turret Cubes**: Neon yellow (#FFFF00) with 40% opacity glow
- **Projectiles**: Bright white (#FFFFFF) with trail
- **UI Elements**: Neon green (#00FF00) for interface

### Geometric Design
- **All Objects**: Perfect cubes only
- **Core Cube**: 2×2×2 units
- **Enemy Cubes**: 1×1×1 units
- **Turret Cubes**: 1×1×1 units
- **No Curves**: Only straight lines and 90-degree angles

### Lighting and Effects
- **Glow Intensity**: 0.3 to 0.5 opacity
- **Glow Blur**: 2-3 pixel blur radius
- **No Shadows**: Flat lighting only
- **Grid Lines**: Optional wireframe grid for spatial reference
- **Particle Effects**: Minimal, only for destruction

### Camera System
- **Perspective**: 3D third-person view
- **Position**: Orbital around center cube
- **Distance**: 25 units from center
- **Controls**: Mouse drag to rotate, scroll to zoom
- **Constraints**: Zoom range 15-40 units from center

## Technical Requirements

### 3D Environment
- **World Size**: 100×100×100 unit cube
- **Gravity**: None (floating objects)
- **Collision Detection**: Axis-aligned bounding boxes
- **Spatial Partitioning**: Octree for performance optimization

### Rendering Pipeline
- **API**: OpenGL 3.3+ or Vulkan
- **Shaders**: Simple vertex/fragment shaders with glow effects
- **Post-processing**: Minimal bloom for neon effects
- **Performance Target**: 60 FPS on GTX 1060 equivalent

### Input System
- **Mouse Controls**: 
  - Left click: Place turret at cursor position
  - Right drag: Rotate camera
  - Scroll: Zoom camera
- **Keyboard Controls**:
  - WASD: Move camera (alternative to mouse)
  - Space: Pause game
  - R: Reset level

### Audio System
- **Sound Effects**:
  - Turret placement: Electronic beep
  - Projectile fire: Energy zap
  - Enemy destruction: Digital explosion
  - Core damage: Warning alarm
- **Music**: Minimal ambient electronic (optional)

## Game Flow

### Start Sequence
1. Display game title "CORE"
2. Show controls tutorial (5 seconds)
3. Spawn first enemy after 3 seconds
4. Begin continuous enemy spawning

### Core Gameplay Loop
1. Player observes incoming enemies from 3D space
2. Player places turrets strategically in 3D environment
3. Turrets automatically target and fire at nearest enemies
4. Enemies destroyed by projectiles or reach center cube
5. Game continues until center cube health reaches 0

### End Conditions
- **Game Over**: Center cube health reaches 0
- **Score**: Based on enemies destroyed and time survived
- **Restart**: Press R to restart immediately

## Future Expansion Framework

### Planned Tower Types
- **Laser Tower**: Continuous beam damage
- **Area Tower**: Explosive damage in radius
- **Slow Tower**: Reduces enemy movement speed

### Planned Enemy Types
- **Fast Enemy**: 2x movement speed, 1 HP
- **Tank Enemy**: 3 HP, 0.5x movement speed
- **Swarm Enemy**: Spawns 3 smaller enemies on death

### Planned Features
- **Power-ups**: Temporary turret upgrades
- **Obstacles**: Destructible environment objects
- **Multiplayer**: Cooperative defense mode

---

*Document Version: 1.0 - Core MVP Specification*
