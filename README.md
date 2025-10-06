# CORE - Minimalist 3D Tower Defense

[🇷🇺 Русская версия](https://github.com/Blckburn/CORE/blob/master/README_RU.md) | [🇺🇸 English Version](#)

![CORE Game](https://img.shields.io/badge/Game-CORE-blue)
![C++](https://img.shields.io/badge/C%2B%2B-17-red)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

## 🎮 Overview

**CORE** is a minimalist 3D Tower Defense game featuring cyberpunk wireframe aesthetics. Players defend a central cube from attacking enemy cubes by placing turrets in three-dimensional space.

### ✨ Key Features

- **🌐 3D Space**: Fully three-dimensional game environment
- **⚡ Cyberpunk Aesthetics**: Neon wireframe objects on black background
- **🏗️ Turret System**: Placement and management of turrets in 3D
- **🎯 Homing Projectiles**: Smart ring-shaped projectiles
- **📷 Camera Control**: Full 3D camera control
- **🎨 Minimalist Design**: Only geometric shapes

## 🚀 Quick Start

### Prerequisites

- **Windows 10/11**
- **Visual Studio 2019+** or **Visual Studio Build Tools**
- **CMake 3.16+**
- **Git**

### Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/Blckburn/CORE.git
   cd CORE
   ```

2. **Initialize vcpkg**:
   ```bash
   cd vcpkg
   ./bootstrap-vcpkg.bat
   ```

3. **Install dependencies**:
   ```bash
   vcpkg install glfw3:x64-windows
   vcpkg install glad:x64-windows
   vcpkg install glm:x64-windows
   ```

4. **Build the project**:
   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build build --config Release
   ```

5. **Run the game**:
   ```bash
   ./build/Release/CORE.exe
   ```

## 🎮 Controls

| Action | Key |
|--------|-----|
| **Camera rotation** | `WASD` or `RMB + mouse movement` |
| **Zoom** | `Mouse wheel` or `Q/E` |
| **Turret placement mode** | `T` |
| **Place turret** | `LMB` (in placement mode) |
| **Adjust placement distance** | `+/-` (in placement mode) |

## 🏗️ Architecture

### Project Structure

```
CORE/
├── src/
│   ├── core/           # Engine core
│   ├── graphics/       # Graphics system
│   ├── game/          # Game logic
│   └── utils/         # Utilities
├── assets/
│   └── shaders/       # Shaders
├── vcpkg/             # Package manager
└── build/             # Build files
```

### Core Components

- **🎯 Engine**: Main engine class
- **🖼️ Renderer**: OpenGL rendering system
- **📷 Camera**: 3D camera with orbital controls
- **👾 Enemy System**: Enemies and spawning
- **🏗️ Turret System**: Turrets and management
- **💥 Projectile System**: Homing projectiles

## 🎨 Visual Style

### Color Scheme

- **Central cube**: Cyan wireframe
- **Enemies**: Red wireframe cubes
- **Turrets**: Green wireframe cubes
- **Projectiles**: Cyan wireframe rings
- **Background**: Pure black

### Cyberpunk Wireframe Aesthetics

- Only straight lines and geometric shapes
- Neon glowing effects
- Minimalist design
- 80s synthwave atmosphere

## 🎯 Game Mechanics

### Game Objective

Defend the central cube from waves of enemy cubes by strategically placing turrets.

### Turret System

- **Range**: 15 units
- **Damage**: 25 HP per shot
- **Fire rate**: 1 shot per second
- **Auto-targeting**: Targets nearest enemy in range

### Enemy System

- **Health**: 100 HP
- **Speed**: 5 units/sec
- **Spawn rate**: 1 enemy per second
- **Behavior**: Move toward center of map

### Projectile System

- **Homing**: Automatic enemy targeting
- **Speed**: 30 units/sec
- **Hit radius**: 1.0 unit
- **Lifetime**: 3 seconds

## 🔧 Technical Details

### Technologies

- **C++17**: Primary programming language
- **OpenGL 3.3**: Graphics API
- **GLFW**: Window and input management
- **GLM**: Mathematics library
- **CMake**: Build system
- **vcpkg**: Package manager

### System Requirements

- **OS**: Windows 10/11
- **GPU**: OpenGL 3.3+ support
- **RAM**: 512 MB
- **Storage**: 100 MB

## 📚 Documentation

- **[Technical Documentation](TECHNICAL_DOCUMENTATION.md)** - Detailed architecture description
- **[Game Design Document](WIKI/ENG_Game_Design_Document.md)** - Game concept
- **[Technical Architecture](WIKI/ENG_Technical_Architecture_Document.md)** - Technical architecture
- **[MVP Roadmap](WIKI/ENG_MVP_Roadmap.md)** - Development plan

## 🚧 Current Status

### ✅ Implemented

- [x] Basic engine
- [x] 3D rendering
- [x] Enemy system
- [x] Turret system
- [x] Projectile system
- [x] Camera controls
- [x] 3D turret placement

### 🚧 In Development

- [ ] Additional turret types
- [ ] Wave system
- [ ] UI interface
- [ ] Sound effects

### 📋 Planned

- [ ] Upgrade system
- [ ] Multiple levels
- [ ] Achievement system
- [ ] Multiplayer

## 🤝 Contributing

We welcome contributions to the CORE project! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## 👨‍💻 Authors

- **Blckburn** - *Main development* - [GitHub](https://github.com/Blckburn)

## 🙏 Acknowledgments

- [GLFW](https://www.glfw.org/) - Window management
- [GLM](https://github.com/g-truc/glm) - Mathematics library
- [OpenGL](https://www.opengl.org/) - Graphics API
- Game development community

## 📞 Contact

- **GitHub Issues**: [Create Issue](https://github.com/Blckburn/CORE/issues)
- **Repository**: [CORE on GitHub](https://github.com/Blckburn/CORE)
- **Discussions**: [Join the Discussion](https://github.com/Blckburn/CORE/discussions)

---

**CORE** - Where Strategy Meets Minimalism 🎯✨