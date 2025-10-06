# CORE Documentation

Welcome to the CORE project documentation! This directory contains comprehensive documentation for the CORE 3D Tower Defense game.

## 📚 Documentation Overview

This documentation suite provides detailed information about the CORE project, covering game design, technical architecture, and implementation details.

### 📋 Available Documents

| Document | Description | Language |
|----------|-------------|----------|
| **[Game Design Document](ENG_Game_Design_Document.md)** | Complete game concept and design specifications | English |
| **[Technical Architecture](ENG_Technical_Architecture_Document.md)** | Detailed technical architecture and system design | English |
| **[MVP Roadmap](ENG_MVP_Roadmap.md)** | Development roadmap and implementation plan | English |
| **[Technical Documentation](TECHNICAL_DOCUMENTATION.md)** | Comprehensive technical documentation | Russian |
| **[Game Design Document](Core_Game_Design_Document.md)** | Game concept and design specifications | Russian |
| **[Technical Architecture](Technical_Architecture_Specification.md)** | Technical architecture specification | Russian |
| **[Implementation Roadmap](Implementation_Roadmap.md)** | Development roadmap | Russian |

## 🎮 Project Overview

**CORE** is a minimalist 3D Tower Defense game featuring cyberpunk wireframe aesthetics. Players defend a central cube from incoming enemy cubes by placing turrets in three-dimensional space.

### Key Features
- **3D Tower Defense**: Unique three-dimensional gameplay
- **Cyberpunk Aesthetics**: Distinctive wireframe visual style
- **Strategic Gameplay**: Complex 3D positioning decisions
- **Modern Engine**: Custom C++ engine with OpenGL rendering

## 🏗️ Architecture Overview

The CORE engine is built with a modular architecture emphasizing:

- **Clean Code**: Modern C++17 practices
- **Performance**: 60 FPS target on standard hardware
- **Extensibility**: Easy addition of new features
- **Maintainability**: Clear separation of concerns

### Core Systems
- **Engine Core**: Main game loop and system coordination
- **Graphics Pipeline**: OpenGL 3.3 rendering system
- **Game Logic**: Entity systems and gameplay mechanics
- **Input Management**: Keyboard and mouse handling
- **3D Interactions**: Ray casting and spatial queries

## 🚀 Quick Start

### Prerequisites
- Windows 10/11
- Visual Studio 2019+ or Build Tools
- CMake 3.16+
- Git

### Building the Project
```bash
# Clone the repository
git clone https://github.com/Blckburn/CORE.git
cd CORE

# Initialize vcpkg
cd vcpkg
./bootstrap-vcpkg.bat

# Install dependencies
vcpkg install glfw3:x64-windows
vcpkg install glad:x64-windows
vcpkg install glm:x64-windows

# Build the project
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release

# Run the game
./build/Release/CORE.exe
```

## 🎯 Gameplay

### Controls
- **WASD**: Camera rotation
- **RMB + Mouse**: Camera rotation
- **Mouse Wheel**: Zoom
- **Q/E**: Zoom
- **T**: Toggle turret placement mode
- **LMB**: Place turret (in placement mode)
- **+/-**: Adjust placement distance

### Game Mechanics
- **Enemy Spawning**: Enemies spawn on sphere around map
- **Turret Placement**: 3D placement with validation rules
- **Combat**: Automatic targeting and homing projectiles
- **Strategy**: Optimize turret positions for maximum efficiency

## 🔧 Technical Details

### Technologies
- **C++17**: Primary programming language
- **OpenGL 3.3**: Graphics rendering
- **GLFW**: Window and input management
- **GLM**: Mathematics library
- **CMake**: Build system
- **vcpkg**: Package management

### Performance
- **Target FPS**: 60 FPS
- **Memory Usage**: <100MB RAM
- **GPU Requirements**: OpenGL 3.3+ support
- **Platform**: Windows 10/11

## 📖 Documentation Structure

### Game Design Documents
- **Concept**: Core game idea and vision
- **Mechanics**: Detailed gameplay systems
- **Visual Design**: TRON aesthetics and art direction
- **User Experience**: Interface and controls

### Technical Documents
- **Architecture**: System design and structure
- **Implementation**: Code organization and patterns
- **Performance**: Optimization strategies
- **Build System**: Compilation and dependencies

### Development Documents
- **Roadmap**: Development timeline and milestones
- **Testing**: Quality assurance strategies
- **Deployment**: Release and distribution
- **Maintenance**: Ongoing development practices

## 🤝 Contributing

We welcome contributions to the CORE project! Please refer to the main repository's contributing guidelines.

### Development Workflow
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

### Code Standards
- Follow C++17 best practices
- Maintain consistent formatting
- Include comprehensive documentation
- Write unit tests for new features

## 📞 Support

### Getting Help
- **GitHub Issues**: Report bugs and request features
- **Discussions**: Ask questions and share ideas
- **Documentation**: Comprehensive technical documentation
- **Community**: Join the development community

### Resources
- **Main Repository**: [GitHub](https://github.com/Blckburn/CORE)
- **Technical Documentation**: See files in this directory
- **Build Instructions**: Included in each document
- **API Reference**: Code comments and documentation

## 📄 License

This project is licensed under the MIT License. See the main repository for details.

## 🙏 Acknowledgments

- **GLFW**: Window and input management
- **GLM**: Mathematics library
- **OpenGL**: Graphics API
- **Game Development Community**: Inspiration and support

---

**CORE** - Where Strategy Meets Minimalism 🎯✨

For the most up-to-date information, please refer to the main repository and the latest documentation versions.
