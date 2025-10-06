#pragma once

#include <glm/glm.hpp>
#include <functional>

// Forward declarations
struct GLFWwindow;

// GLFW constants (since we can't include the header in .h file)
#ifndef GLFW_KEY_LAST
#define GLFW_KEY_LAST 512
#endif
#ifndef GLFW_MOUSE_BUTTON_LAST
#define GLFW_MOUSE_BUTTON_LAST 8
#endif

class InputManager {
public:
    InputManager(GLFWwindow* window);
    ~InputManager();
    
    void Update();
    
    // Keyboard input
    bool IsKeyPressed(int key) const;
    bool IsKeyJustPressed(int key) const;
    bool IsKeyReleased(int key) const;
    
    // Mouse input
    glm::vec2 GetMousePosition() const;
    glm::vec2 GetMouseDelta() const;
    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonJustPressed(int button) const;
    float GetScrollDelta() const;
    void ConsumeScrollDelta();
    
    // Callbacks
    void SetKeyCallback(std::function<void(int, int)> callback);
    void SetMouseCallback(std::function<void(double, double)> callback);
    void SetScrollCallback(std::function<void(double)> callback);
    
private:
    GLFWwindow* window_;
    glm::vec2 mouse_position_;
    glm::vec2 mouse_delta_;
    glm::vec2 last_mouse_position_;
    float scroll_delta_;
    
    // Key state tracking
    bool keys_[GLFW_KEY_LAST];
    bool keys_prev_[GLFW_KEY_LAST];
    
    // Mouse state tracking
    bool mouse_buttons_[GLFW_MOUSE_BUTTON_LAST];
    bool mouse_buttons_prev_[GLFW_MOUSE_BUTTON_LAST];
    
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseCallback(GLFWwindow* window, double x, double y);
    static void ScrollCallback(GLFWwindow* window, double x, double y);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};
