# CORE
## Minimalist 3D Tower Defense Game

---

## Project Overview

**CORE** is a minimalist 3D tower defense game featuring a cyberpunk aesthetic inspired by the TRON universe. Players defend a central cube from incoming enemy cubes that attack from all three-dimensional planes, strategically placing defensive turrets in 3D space.

### Key Features
- **3D Spatial Defense**: Enemies approach from all directions in true 3D space
- **TRON-Inspired Aesthetic**: Neon glows, geometric shapes, pure black void
- **Minimalist Design**: Only cubes and straight lines, gameplay-focused
- **Strategic Depth**: 3D positioning and tactical turret placement

---

## Documentation

### Core Documents
1. **[Game Design Document](Core_Game_Design_Document.md)** - Complete game mechanics, visual style, and technical requirements
2. **[Technical Architecture](Technical_Architecture_Specification.md)** - Engine architecture, ECS system, and implementation details  
3. **[Implementation Roadmap](Implementation_Roadmap.md)** - 12-week development plan with milestones and deliverables

### Project Structure
```
CORE/
├── src/                    # Source code
├── assets/                 # Game assets
├── docs/                   # Documentation
└── build/                  # Build output
```

---

## Quick Start

### Game Concept
- **Objective**: Defend the central cyan cube from red enemy cubes
- **Controls**: Mouse drag to rotate camera, click to place yellow turrets
- **Strategy**: Position turrets strategically in 3D space to intercept enemies
- **Progression**: Enemies spawn faster and become stronger over time

### Technical Specs
- **Platform**: Windows, Linux
- **Graphics**: OpenGL 3.3+
- **Performance**: 60 FPS target on GTX 1060
- **Size**: < 50 MB executable, < 200 MB RAM

---

## Development Status

**Current Phase**: Pre-development planning  
**Timeline**: 12 weeks to MVP  
**Team Size**: 1-2 developers  

### Next Steps
1. Set up development environment
2. Implement basic 3D rendering pipeline
3. Create core ECS system
4. Build first playable prototype

---

## Visual Style

### Color Palette
- **Background**: Pure black (#000000)
- **Core Cube**: Neon cyan (#00FFFF)
- **Enemy Cubes**: Neon red (#FF0040)
- **Turret Cubes**: Neon yellow (#FFFF00)
- **Projectiles**: Bright white (#FFFFFF)

### Design Principles
- Only geometric shapes (cubes)
- Straight lines and 90-degree angles
- Neon glow effects
- Minimalist, gameplay-focused aesthetic

---

*CORE - Where strategy meets cyberpunk minimalism*

