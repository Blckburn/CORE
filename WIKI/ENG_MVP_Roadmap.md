# Implementation Roadmap: CORE

## Project Overview

**CORE** is a minimalist 3D Tower Defense game featuring cyberpunk wireframe aesthetics. This roadmap outlines the development plan for the first Minimum Viable Product (MVP) version, focusing on core gameplay mechanics and technical foundation.

## MVP Goals

### Primary Objectives
1. **Functional 3D Tower Defense gameplay**
2. **Cyberpunk wireframe visual aesthetics**
3. **Smooth 60 FPS performance**
4. **Intuitive 3D controls**
5. **Complete game loop**

### Success Criteria
- Players can place turrets in 3D space
- Enemies spawn and move towards the center
- Turrets automatically target and destroy enemies
- Game runs smoothly on target hardware
- Visual style matches cyberpunk wireframe aesthetic

## Development Phases

### Phase 1: Core Engine Foundation (Weeks 1-2)

#### 1.1 Project Setup
- [x] Initialize CMake project structure
- [x] Set up vcpkg package management
- [x] Configure build system
- [x] Set up version control

#### 1.2 Basic Engine Components
- [x] Window creation with GLFW
- [x] OpenGL 3.3 context initialization
- [x] Basic rendering pipeline
- [x] Input handling system
- [x] Time management system

#### 1.3 Graphics Foundation
- [x] Shader compilation system
- [x] Basic mesh rendering
- [x] Camera system implementation
- [x] Wireframe rendering mode

**Deliverables**:
- Functional window with OpenGL context
- Basic rendering of geometric shapes
- Input handling for keyboard and mouse

### Phase 2: 3D Rendering System (Weeks 3-4)

#### 2.1 3D Graphics Pipeline
- [x] 3D transformation matrices
- [x] Perspective projection
- [x] Depth testing
- [x] Wireframe cube rendering

#### 2.2 Camera System
- [x] Orbital camera control
- [x] Mouse-based rotation
- [x] Zoom functionality
- [x] Smooth camera movement

#### 2.3 Ray Casting System
- [x] Screen-to-world coordinate conversion
- [x] 3D plane intersection
- [x] Ground intersection calculation
- [x] 3D placement plane system

**Deliverables**:
- Fully functional 3D camera
- 3D wireframe cube rendering
- 3D interaction system

### Phase 3: Game Logic Foundation (Weeks 5-6)

#### 3.1 Entity System
- [x] Base entity class
- [x] Transform management
- [x] Lifecycle management
- [x] Update and render loops

#### 3.2 Enemy System
- [x] Enemy entity implementation
- [x] Movement towards center
- [x] Health and damage system
- [x] Collision detection

#### 3.3 Enemy Spawning
- [x] Random spawn positions on sphere
- [x] Configurable spawn rates
- [x] Enemy lifecycle management
- [x] Spawn radius configuration

**Deliverables**:
- Functional enemy entities
- Enemy spawning system
- Basic collision detection

### Phase 4: Turret System (Weeks 7-8)

#### 4.1 Turret Implementation
- [x] Turret entity class
- [x] Turret placement system
- [x] Placement validation rules
- [x] Visual feedback system

#### 4.2 Turret Placement
- [x] 3D placement in space
- [x] Placement constraints
- [x] Preview system
- [x] Placement confirmation

#### 4.3 Turret Management
- [x] Turret collection management
- [x] Placement validation
- [x] Distance calculations
- [x] Turret state management

**Deliverables**:
- Functional turret placement
- 3D placement validation
- Visual placement feedback

### Phase 5: Combat System (Weeks 9-10)

#### 5.1 Targeting System
- [x] Automatic enemy targeting
- [x] Range-based targeting
- [x] Nearest enemy selection
- [x] Target tracking

#### 5.2 Projectile System
- [x] Projectile entity implementation
- [x] Projectile physics
- [x] Homing behavior
- [x] Collision detection

#### 5.3 Combat Mechanics
- [x] Damage application
- [x] Enemy death handling
- [x] Projectile lifetime
- [x] Hit detection

**Deliverables**:
- Functional combat system
- Homing projectiles
- Complete damage system

### Phase 6: Polish and Optimization (Weeks 11-12)

#### 6.1 Visual Polish
- [x] TRON color scheme implementation
- [x] Wireframe rendering optimization
- [x] Visual feedback improvements
- [x] Aesthetic refinements

#### 6.2 Performance Optimization
- [x] Object pooling for projectiles
- [x] Efficient rendering pipeline
- [x] Memory management optimization
- [x] Frame rate optimization

#### 6.3 User Experience
- [x] Intuitive controls
- [x] Responsive input handling
- [x] Smooth camera movement
- [x] Visual feedback systems

**Deliverables**:
- Polished visual presentation
- Optimized performance
- Smooth user experience

## Technical Implementation Details

### Core Systems Architecture

```
CORE/
├── src/
│   ├── main.cpp                 # Entry point
│   ├── core/                    # Engine core
│   │   ├── engine.h/cpp         # Main engine class
│   │   ├── window.h/cpp         # Window management
│   │   ├── input.h/cpp          # Input handling
│   │   └── time.h/cpp           # Time management
│   ├── graphics/                # Graphics system
│   │   ├── renderer.h/cpp       # Rendering
│   │   ├── camera.h/cpp         # Camera
│   │   ├── shader.h/cpp         # Shaders
│   │   ├── mesh.h/cpp           # 3D meshes
│   │   └── ray_caster.h/cpp     # Ray casting
│   ├── game/                    # Game logic
│   │   ├── game.h/cpp           # Main game class
│   │   ├── entity.h/cpp         # Base entity
│   │   ├── enemy.h/cpp          # Enemies
│   │   ├── enemy_spawner.h/cpp  # Enemy spawning
│   │   ├── turret.h/cpp         # Turrets
│   │   ├── turret_manager.h/cpp # Turret management
│   │   ├── turret_preview.h/cpp # Turret preview
│   │   ├── projectile.h/cpp     # Projectiles
│   │   └── projectile_manager.h/cpp # Projectile management
│   └── utils/                   # Utilities
│       └── math.h/cpp           # Math functions
├── assets/
│   └── shaders/                 # Shaders
│       ├── basic.vert           # Vertex shader
│       └── basic.frag           # Fragment shader
└── CMakeLists.txt               # Build configuration
```

### Key Technical Decisions

#### 1. Graphics API Choice
- **OpenGL 3.3**: Cross-platform compatibility
- **GLFW**: Window and input management
- **GLM**: Mathematics library
- **GLAD**: OpenGL loader

#### 2. Architecture Pattern
- **Component-based**: Modular system design
- **Entity-System**: Game object management
- **Smart Pointers**: Modern C++ memory management
- **RAII**: Resource management

#### 3. Performance Strategy
- **Object Pooling**: Efficient memory usage
- **Batch Rendering**: Optimized draw calls
- **Wireframe Rendering**: Simplified graphics
- **Efficient Collision**: Simple spherical detection

### Gameplay Mechanics

#### Core Gameplay Loop
1. **Enemy Spawning**: Enemies spawn on sphere around map
2. **Enemy Movement**: Direct path to center cube
3. **Turret Placement**: Player places turrets in 3D space
4. **Combat**: Turrets automatically target and fire
5. **Defense**: Prevent enemies from reaching center

#### Turret Placement Rules
- **Minimum distance from center**: 5 units
- **Maximum distance from center**: 20 units
- **Minimum distance between turrets**: 3 units
- **3D placement**: Any position in 3D space

#### Combat System
- **Turret range**: 15 units
- **Turret damage**: 25 HP per shot
- **Fire rate**: 1 shot per second
- **Projectile speed**: 30 units/second
- **Hit radius**: 1.0 unit

### Visual Design

#### Cyberpunk Wireframe Aesthetic
- **Color scheme**: Cyan, red, green on black background
- **Shapes**: Only geometric wireframe objects
- **Effects**: Subtle neon glow effects
- **Style**: Minimalist, cyberpunk, 80s synthwave-inspired

#### Object Colors
- **Central cube**: Cyan wireframe
- **Enemies**: Red wireframe cubes
- **Turrets**: Green wireframe cubes
- **Projectiles**: Cyan wireframe rings
- **Background**: Pure black void

## Testing Strategy

### Unit Testing
- **Entity system**: Individual component testing
- **Math utilities**: Mathematical function validation
- **Collision detection**: Hit detection accuracy
- **Input handling**: Input state management

### Integration Testing
- **Graphics pipeline**: Rendering system integration
- **Game loop**: Complete game cycle testing
- **Performance**: Frame rate and memory usage
- **User interaction**: Control responsiveness

### Performance Testing
- **Frame rate**: 60 FPS target
- **Memory usage**: <100MB RAM
- **Load times**: <3 seconds startup
- **Stability**: Long-term play testing

## Risk Assessment

### Technical Risks
- **OpenGL compatibility**: Mitigated by GLAD loader
- **Performance issues**: Addressed through optimization
- **Memory leaks**: Prevented by smart pointers
- **Platform compatibility**: Tested on target platforms

### Development Risks
- **Scope creep**: Strict MVP focus
- **Timeline delays**: Buffer time included
- **Technical complexity**: Phased development approach
- **Resource constraints**: Efficient development practices

## Success Metrics

### Technical Metrics
- **Performance**: 60 FPS on target hardware
- **Stability**: Crash-free gameplay
- **Memory**: Efficient memory usage
- **Load time**: Quick startup

### Gameplay Metrics
- **Engagement**: Players continue playing
- **Strategy depth**: Multiple viable strategies
- **Difficulty**: Appropriate challenge level
- **Replayability**: Long-term engagement

## Phase 7: Wave System (Weeks 13-14) 🔥 IN DEVELOPMENT

### 7.1 Wave System Foundation
- [ ] WaveManager class
- [ ] Wave configuration
- [ ] Wave progression mechanics
- [ ] Wave state controller

### 7.2 Wave Gameplay Mechanics
- [ ] Timer between waves
- [ ] Wave difficulty scaling
- [ ] Enemy count increase
- [ ] Enemy stat increases
- [ ] Wave reward system

### 7.3 Integration and Balancing
- [ ] Enemy Spawner integration
- [ ] Difficulty progression balancing
- [ ] Difficulty curve testing
- [ ] Wave visual feedback

**Results**:
- Structured wave system
- Progressive difficulty
- Clear pauses between waves
- Sense of progression

## Phase 8: User Interface (Weeks 15-16)

### 8.1 Core HUD
- [ ] Text rendering system
- [ ] Wave number display
- [ ] Time until next wave counter
- [ ] Core health display
- [ ] Score/resource counter

### 8.2 Game Information
- [ ] Turret information
- [ ] Placement status
- [ ] Cooldown indicators
- [ ] Game statistics

### 8.3 UI Menus
- [ ] Main menu
- [ ] Pause menu
- [ ] Game over screen
- [ ] Menu navigation

**Results**:
- Full functional UI
- Clear player feedback
- Professional presentation

## Phase 9: Content Expansion (Weeks 17-20)

### 9.1 Additional Turret Types
- [ ] Rapid-fire turret (low damage, high fire rate)
- [ ] Sniper turret (high damage, low fire rate, long range)
- [ ] AoE turret (area damage)
- [ ] Slow turret (speed debuff)

### 9.2 Enemy Variety
- [ ] Fast enemies (less HP, higher speed)
- [ ] Tank enemies (more HP, slower)
- [ ] Flying enemies
- [ ] Boss enemies

### 9.3 Upgrade System
- [ ] Upgrade currency/points
- [ ] Turret upgrade menu
- [ ] Temporary power-ups
- [ ] Permanent upgrades

**Results**:
- Strategic variety
- Gameplay depth
- Progression system

## Phase 10: Audio and Polish (Weeks 21-22)

### 10.1 Sound System
- [ ] Audio engine integration (OpenAL/miniaudio)
- [ ] Shooting sound effects
- [ ] Explosion and hit sounds
- [ ] UI sounds
- [ ] Synthwave background music

### 10.2 Visual Polish
- [ ] Particle effects on destruction
- [ ] Enhanced visual effects
- [ ] Transition animations
- [ ] Lighting improvements

### 10.3 Final Optimization
- [ ] Performance profiling
- [ ] Bottleneck optimization
- [ ] Stability testing
- [ ] Bug fixes

**Results**:
- Fully immersive experience
- Professional audiovisual presentation
- Optimal performance

## Long-term Vision (3-6 months)

### Extended Content
- **Multiple levels**: Different maps and environments
- **Achievement system**: Goals and rewards
- **Leaderboards**: Competitive elements
- **Difficulty modes**: Adjustable challenge

### Technical Improvements
- **Multiplayer**: Cooperative gameplay
- **Level editor**: Custom map creation
- **Mod support**: Community content
- **Platform expansion**: Additional platforms
- **VR support**: Virtual reality adaptation

## Conclusion

The CORE MVP roadmap provides a structured approach to developing a functional 3D Tower Defense game with TRON aesthetics. The phased development approach ensures steady progress while maintaining focus on core gameplay mechanics and technical excellence.

Key success factors:
- **Clear technical architecture**
- **Phased development approach**
- **Performance optimization focus**
- **User experience emphasis**
- **Extensible design for future features**

The roadmap balances technical complexity with achievable milestones, ensuring the MVP delivers a complete and engaging gameplay experience while providing a solid foundation for future development.
