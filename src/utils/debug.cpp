#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "debug.h"
#include <iostream>

namespace Debug {
    void Log(const std::string& message) {
        std::cout << "[LOG] " << message << std::endl;
    }
    
    void LogWarning(const std::string& message) {
        std::cout << "[WARNING] " << message << std::endl;
    }
    
    void LogError(const std::string& message) {
        std::cerr << "[ERROR] " << message << std::endl;
    }
    
    void PrintOpenGLInfo() {
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
        std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    }
    
    void CheckOpenGLError(const std::string& operation) {
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error in " << operation << ": " << error << std::endl;
        }
    }
}
