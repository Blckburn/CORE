#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "input.h"
#include <cstring>
#include <iostream>

InputManager::InputManager(GLFWwindow* window) 
    : window_(window)
    , mouse_position_(0.0f, 0.0f)
    , mouse_delta_(0.0f, 0.0f)
    , last_mouse_position_(0.0f, 0.0f)
    , scroll_delta_(0.0f) {
    
    // Initialize key arrays
    memset(keys_, false, sizeof(keys_));
    memset(keys_prev_, false, sizeof(keys_prev_));
    memset(mouse_buttons_, false, sizeof(mouse_buttons_));
    memset(mouse_buttons_prev_, false, sizeof(mouse_buttons_prev_));
    
    // Set GLFW callbacks
    glfwSetWindowUserPointer(window_, this);
    glfwSetKeyCallback(window_, KeyCallback);
    glfwSetCursorPosCallback(window_, MouseCallback);
    glfwSetScrollCallback(window_, ScrollCallback);
    glfwSetMouseButtonCallback(window_, MouseButtonCallback);
    
    // Enable cursor capture for better mouse input
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

InputManager::~InputManager() {
}

void InputManager::Update() {
    // Update previous key states
    memcpy(keys_prev_, keys_, sizeof(keys_));
    memcpy(mouse_buttons_prev_, mouse_buttons_, sizeof(mouse_buttons_));
    
    // Update mouse delta
    mouse_delta_ = mouse_position_ - last_mouse_position_;
    last_mouse_position_ = mouse_position_;
    
    // Note: scroll_delta_ is NOT reset here - it accumulates until consumed
}

bool InputManager::IsKeyPressed(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return false;
    return keys_[key];
}

bool InputManager::IsKeyJustPressed(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return false;
    return keys_[key] && !keys_prev_[key];
}

bool InputManager::IsKeyReleased(int key) const {
    if (key < 0 || key >= GLFW_KEY_LAST) return false;
    return !keys_[key] && keys_prev_[key];
}

glm::vec2 InputManager::GetMousePosition() const {
    return mouse_position_;
}

glm::vec2 InputManager::GetMousePositionFramebuffer() const {
    return mouse_position_fb_;
}

glm::vec2 InputManager::GetMouseDelta() const {
    return mouse_delta_;
}

bool InputManager::IsMouseButtonPressed(int button) const {
    if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST) return false;
    return mouse_buttons_[button];
}

bool InputManager::IsMouseButtonJustPressed(int button) const {
    if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST) return false;
    bool result = mouse_buttons_[button] && !mouse_buttons_prev_[button];
    if (result) {
        std::cout << "DEBUG: Mouse button " << button << " just pressed! (current=" << mouse_buttons_[button] << ", prev=" << mouse_buttons_prev_[button] << ")" << std::endl;
    }
    return result;
}

float InputManager::GetScrollDelta() const {
    return scroll_delta_;
}

void InputManager::ConsumeScrollDelta() {
    scroll_delta_ = 0.0f;
}

void InputManager::SetKeyCallback(std::function<void(int, int)> callback) {
    // Store callback for later use if needed
}

void InputManager::SetMouseCallback(std::function<void(double, double)> callback) {
    // Store callback for later use if needed
}

void InputManager::SetScrollCallback(std::function<void(double)> callback) {
    // Store callback for later use if needed
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;
    
    if (key >= 0 && key < GLFW_KEY_LAST) {
        if (action == GLFW_PRESS) {
            input->keys_[key] = true;
            std::cout << "Key pressed: " << key << std::endl;
        } else if (action == GLFW_RELEASE) {
            input->keys_[key] = false;
        }
    }
}

void InputManager::MouseCallback(GLFWwindow* window, double x, double y) {
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;
    
    input->mouse_position_ = glm::vec2(static_cast<float>(x), static_cast<float>(y));
    // Map to framebuffer coordinates (accounts for DPI scaling on Windows)
    int win_w = 0, win_h = 0, fb_w = 0, fb_h = 0;
    glfwGetWindowSize(window, &win_w, &win_h);
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    float scale_x = (win_w > 0) ? static_cast<float>(fb_w) / static_cast<float>(win_w) : 1.0f;
    float scale_y = (win_h > 0) ? static_cast<float>(fb_h) / static_cast<float>(win_h) : 1.0f;
    input->mouse_position_fb_ = glm::vec2(static_cast<float>(x) * scale_x, static_cast<float>(y) * scale_y);
    
    // Debug mouse position (only occasionally)
    static int counter = 0;
    if (++counter % 60 == 0) { // Every 60 frames
        std::cout << "Mouse position: " << x << ", " << y << std::endl;
    }
}

void InputManager::ScrollCallback(GLFWwindow* window, double x, double y) {
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;
    
    input->scroll_delta_ = static_cast<float>(y);
    std::cout << "Scroll: " << y << std::endl;
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    InputManager* input = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (!input) return;
    
    if (button >= 0 && button < GLFW_MOUSE_BUTTON_LAST) {
        if (action == GLFW_PRESS) {
            input->mouse_buttons_[button] = true;
            std::cout << "Mouse button pressed: " << button << std::endl;
        } else if (action == GLFW_RELEASE) {
            input->mouse_buttons_[button] = false;
            std::cout << "Mouse button released: " << button << std::endl;
        }
    }
}
