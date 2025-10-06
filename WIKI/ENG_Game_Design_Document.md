# Game Design Document: CORE

## 1. Game Overview

### 1.1 Core Concept
**CORE** is a minimalist 3D Tower Defense game featuring cyberpunk wireframe aesthetics. Players defend a central cube from incoming enemy cubes that attack from all three-dimensional planes. The game emphasizes strategic gameplay over visual complexity, featuring a cyberpunk aesthetic inspired by 80s synthwave culture with neon glows, geometric shapes, and a pure black void environment.

### 1.2 Genre
- **Primary**: 3D Tower Defense
- **Secondary**: Strategy, Action
- **Aesthetic**: Minimalist, Cyberpunk, 80s synthwave-inspired

### 1.3 Target Audience
- **Primary**: Strategy game enthusiasts (18-35 years)
- **Secondary**: Cyberpunk enthusiasts, minimalist design lovers
- **Platform**: PC (Windows)

### 1.4 Unique Selling Points
- **3D Tower Defense**: Unique three-dimensional gameplay
- **Cyberpunk Aesthetics**: Distinctive visual style
- **Minimalist Design**: Clean, focused gameplay
- **Strategic Depth**: Complex 3D positioning decisions

## 2. Game Mechanics

### 2.1 Core Gameplay Loop
1. **Enemy Spawning**: Enemies spawn randomly on a sphere around the map
2. **Enemy Movement**: Enemies move towards the central cube
3. **Turret Placement**: Players place turrets in 3D space
4. **Combat**: Turrets automatically target and fire at enemies
5. **Defense**: Players must prevent enemies from reaching the center
6. **Strategy**: Players optimize turret placement for maximum efficiency

### 2.2 Turret Placement System
- **3D Placement**: Turrets can be placed anywhere in 3D space
- **Placement Rules**: 
  - Minimum distance from center: 5 units
  - Maximum distance from center: 20 units
  - Minimum distance between turrets: 3 units
- **Visual Feedback**: Green preview for valid placement, red for invalid
- **Dynamic Plane**: Placement plane adjusts based on camera position

### 2.3 Combat System
- **Automatic Targeting**: Turrets target nearest enemy in range
- **Projectile System**: Homing projectiles in ring form
- **Damage System**: 25 HP damage per hit
- **Range System**: 15 unit range for turrets
- **Fire Rate**: 1 shot per second

### 2.4 Enemy System
- **Spawn Mechanics**: Random spawn on sphere (25 unit radius)
- **Movement**: Direct path to center cube
- **Health**: 100 HP per enemy
- **Speed**: 5 units per second
- **Spawn Rate**: 1 enemy per second

## 3. Visual Design

### 3.1 Art Style
- **Aesthetic**: TRON-inspired, minimalist, cyberpunk
- **Shapes**: Only straight lines and geometric cube shapes
- **Effects**: Glowing effects for all game elements
- **Color Palette**: Pure black background, neon colors

### 3.2 Color Scheme
- **Central Cube**: Cyan wireframe
- **Enemies**: Red wireframe cubes
- **Turrets**: Green wireframe cubes
- **Projectiles**: Cyan wireframe rings
- **Background**: Pure black void

### 3.3 Visual Effects
- **Wireframe Rendering**: Only edges visible
- **Neon Glow**: Subtle glow effects
- **Minimalist UI**: Clean, functional interface
- **3D Perspective**: Full three-dimensional view

## 4. Technical Specifications

### 4.1 Engine Architecture
- **Language**: C++17
- **Graphics**: OpenGL 3.3
- **Window Management**: GLFW
- **Mathematics**: GLM
- **Build System**: CMake

### 4.2 Performance Requirements
- **Target FPS**: 60 FPS
- **Memory Usage**: ~50MB RAM
- **GPU Requirements**: OpenGL 3.3+ support
- **Platform**: Windows 10/11

### 4.3 Controls
- **Camera Rotation**: WASD or RMB + mouse
- **Zoom**: Mouse wheel or Q/E
- **Turret Placement**: T key + LMB
- **Placement Distance**: +/- keys

## 5. Game Flow

### 5.1 Main Menu
- **Title**: "CORE"
- **Start Game**: Begin gameplay
- **Settings**: Game configuration
- **Exit**: Quit application

### 5.2 Gameplay
- **Initial State**: Empty map with central cube
- **Enemy Spawning**: Continuous enemy spawning
- **Player Actions**: Turret placement and camera control
- **Victory Condition**: Survive indefinitely
- **Defeat Condition**: Enemy reaches center cube

### 5.3 Game States
- **Playing**: Active gameplay
- **Paused**: Game paused
- **Game Over**: Enemy reached center
- **Victory**: Survived wave (future feature)

## 6. Audio Design

### 6.1 Sound Effects
- **Turret Fire**: Electronic "pew" sound
- **Enemy Hit**: Metallic impact sound
- **Enemy Death**: Electronic disintegration
- **Placement**: Confirmation beep

### 6.2 Music
- **Ambient**: Electronic ambient tracks
- **Combat**: Intensified electronic beats
- **Style**: 80s synthwave-inspired electronic music

## 7. User Interface

### 7.1 HUD Elements
- **Health Indicator**: Center cube health (future)
- **Enemy Counter**: Number of active enemies
- **Turret Count**: Number of placed turrets
- **Score**: Points earned (future)

### 7.2 Menus
- **Main Menu**: Simple, minimalist design
- **Pause Menu**: Overlay with options
- **Settings Menu**: Graphics and audio options

## 8. Progression System

### 8.1 Current Implementation
- **Basic Gameplay**: Core mechanics implemented
- **Continuous Play**: Infinite enemy spawning
- **No Progression**: Pure survival gameplay

### 8.2 Future Features
- **Wave System**: Increasing difficulty waves
- **Turret Upgrades**: Enhanced turret capabilities
- **Multiple Levels**: Different map layouts
- **Achievements**: Unlockable content

## 9. Technical Features

### 9.1 3D Engine
- **Full 3D Rendering**: Complete three-dimensional environment
- **Orbital Camera**: Smooth camera control
- **Ray Casting**: 3D interaction system
- **Wireframe Rendering**: Optimized rendering pipeline

### 9.2 Physics
- **Collision Detection**: Spherical collision system
- **Projectile Physics**: Homing projectile behavior
- **Spatial Queries**: Efficient enemy targeting

### 9.3 Performance
- **Object Pooling**: Efficient memory management
- **Batch Rendering**: Optimized draw calls
- **Frustum Culling**: Invisible object culling

## 10. Future Development

### 10.1 Short-term Goals
- **Wave System**: Implement enemy waves
- **UI Improvements**: Better user interface
- **Sound Integration**: Audio system
- **Performance Optimization**: Further optimizations

### 10.2 Long-term Goals
- **Multiple Turret Types**: Different turret varieties
- **Power-ups**: Temporary enhancements
- **Multiplayer**: Cooperative gameplay
- **Level Editor**: Custom map creation

## 11. Success Metrics

### 11.1 Technical Metrics
- **Performance**: 60 FPS on target hardware
- **Stability**: Crash-free gameplay
- **Memory**: Efficient memory usage
- **Load Times**: Quick game startup

### 11.2 Gameplay Metrics
- **Engagement**: Players continue playing
- **Strategy Depth**: Multiple viable strategies
- **Difficulty Curve**: Appropriate challenge level
- **Replayability**: Long-term engagement

## 12. Conclusion

CORE represents a unique take on the Tower Defense genre, combining 3D gameplay with minimalist cyberpunk wireframe aesthetics. The game focuses on strategic depth through 3D positioning while maintaining visual simplicity and clean design. The current implementation provides a solid foundation for future development and expansion of features.

The game successfully demonstrates:
- **Innovative 3D Tower Defense mechanics**
- **Distinctive visual style**
- **Clean, focused gameplay**
- **Technical excellence**
- **Extensibility for future features**

CORE is positioned to appeal to strategy game enthusiasts and fans of minimalist design, offering a fresh perspective on the Tower Defense genre.
