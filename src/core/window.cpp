#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include <iostream>
#include <stdexcept>

Window::Window() : window_(nullptr), width_(0), height_(0) {
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
        window_ = nullptr;
    }
}

bool Window::Initialize(int width, int height, const std::string& title) {
    std::cout << "Initializing window: " << width << "x" << height << " - " << title << std::endl;
    
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return false;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create window
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window_) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;
    }
    
    // Make context current
    glfwMakeContextCurrent(window_);
    
    // Set window properties
    width_ = width;
    height_ = height;
    title_ = title;
    
    // Set callbacks
    glfwSetWindowUserPointer(window_, this);
    glfwSetFramebufferSizeCallback(window_, FramebufferSizeCallback);
    
    // Enable VSync
    glfwSwapInterval(1);
    
    // Load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
        return false;
    }
    
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    return true;
}

void Window::SwapBuffers() {
    if (window_) {
        glfwSwapBuffers(window_);
    }
}

void Window::PollEvents() {
    glfwPollEvents();
}

bool Window::ShouldClose() const {
    return window_ ? !glfwWindowShouldClose(window_) : true;
}

void Window::SetShouldClose(bool should_close) {
    if (window_) {
        glfwSetWindowShouldClose(window_, should_close ? GLFW_TRUE : GLFW_FALSE);
    }
}

void Window::SetKeyCallback(GLFWkeyfun callback) {
    if (window_) {
        glfwSetKeyCallback(window_, callback);
    }
}

void Window::SetMouseCallback(GLFWcursorposfun callback) {
    if (window_) {
        glfwSetCursorPosCallback(window_, callback);
    }
}

void Window::SetScrollCallback(GLFWscrollfun callback) {
    if (window_) {
        glfwSetScrollCallback(window_, callback);
    }
}

void Window::SetResizeCallback(GLFWframebuffersizefun callback) {
    if (window_) {
        glfwSetFramebufferSizeCallback(window_, callback);
    }
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->width_ = width;
        win->height_ = height;
        glViewport(0, 0, width, height);
        std::cout << "Window resized to: " << width << "x" << height << std::endl;
    }
}
