#pragma once

#include <string>

// Forward declarations
struct GLFWwindow;
typedef void (*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
typedef void (*GLFWcursorposfun)(GLFWwindow*, double, double);
typedef void (*GLFWscrollfun)(GLFWwindow*, double, double);
typedef void (*GLFWframebuffersizefun)(GLFWwindow*, int, int);

class Window {
public:
    Window();
    ~Window();
    
    // Initialize window
    bool Initialize(int width, int height, const std::string& title);
    
    // Window operations
    void SwapBuffers();
    void PollEvents();
    bool ShouldClose() const;
    void SetShouldClose(bool should_close);
    
    // Getters
    GLFWwindow* GetGLFWWindow() const { return window_; }
    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }
    float GetAspectRatio() const { return static_cast<float>(width_) / height_; }
    
    // Input callbacks
    void SetKeyCallback(GLFWkeyfun callback);
    void SetMouseCallback(GLFWcursorposfun callback);
    void SetScrollCallback(GLFWscrollfun callback);
    void SetResizeCallback(GLFWframebuffersizefun callback);
    
private:
    GLFWwindow* window_;
    int width_;
    int height_;
    std::string title_;
    
    // Static callback functions
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
};
