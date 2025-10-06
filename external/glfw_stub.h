#ifndef GLFW_STUB_H
#define GLFW_STUB_H

// Simple GLFW stub for development
// This allows compilation without GLFW installed

#define GLFW_TRUE 1
#define GLFW_FALSE 0

// Key codes
#define GLFW_KEY_ESCAPE 256
#define GLFW_KEY_SPACE 32
#define GLFW_KEY_W 87
#define GLFW_KEY_A 65
#define GLFW_KEY_S 83
#define GLFW_KEY_D 68
#define GLFW_KEY_LAST 512

// Mouse buttons
#define GLFW_MOUSE_BUTTON_LEFT 0
#define GLFW_MOUSE_BUTTON_RIGHT 1
#define GLFW_MOUSE_BUTTON_LAST 8

// Window hints
#define GLFW_CONTEXT_VERSION_MAJOR 0x00022002
#define GLFW_CONTEXT_VERSION_MINOR 0x00022003
#define GLFW_OPENGL_PROFILE 0x00022008
#define GLFW_OPENGL_CORE_PROFILE 0x00032001
#define GLFW_OPENGL_FORWARD_COMPAT 0x00022006

// Actions
#define GLFW_PRESS 1
#define GLFW_RELEASE 0

typedef struct GLFWwindow GLFWwindow;
typedef void (*GLFWkeyfun)(GLFWwindow*, int, int, int, int);
typedef void (*GLFWcursorposfun)(GLFWwindow*, double, double);
typedef void (*GLFWscrollfun)(GLFWwindow*, double, double);
typedef void (*GLFWframebuffersizefun)(GLFWwindow*, int, int);
typedef void* (*GLADloadproc)(const char*);

// Function declarations (stubs)
int glfwInit();
void glfwTerminate();
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share);
void glfwDestroyWindow(GLFWwindow* window);
void glfwMakeContextCurrent(GLFWwindow* window);
void glfwSwapBuffers(GLFWwindow* window);
void glfwPollEvents();
int glfwWindowShouldClose(GLFWwindow* window);
void glfwSetWindowShouldClose(GLFWwindow* window, int value);
void glfwSwapInterval(int interval);
void* glfwGetProcAddress(const char* procname);
void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer);
void* glfwGetWindowUserPointer(GLFWwindow* window);
void glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback);
void glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback);
void glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback);
void glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun callback);
void glfwWindowHint(int hint, int value);

// Stub implementations
int glfwInit() { return 1; }
void glfwTerminate() {}
GLFWwindow* glfwCreateWindow(int width, int height, const char* title, void* monitor, void* share) { return (GLFWwindow*)0x1; }
void glfwDestroyWindow(GLFWwindow* window) {}
void glfwMakeContextCurrent(GLFWwindow* window) {}
void glfwSwapBuffers(GLFWwindow* window) {}
void glfwPollEvents() {}
int glfwWindowShouldClose(GLFWwindow* window) { return 0; }
void glfwSetWindowShouldClose(GLFWwindow* window, int value) {}
void glfwSwapInterval(int interval) {}
void* glfwGetProcAddress(const char* procname) { return nullptr; }
void glfwSetWindowUserPointer(GLFWwindow* window, void* pointer) {}
void* glfwGetWindowUserPointer(GLFWwindow* window) { return nullptr; }
void glfwSetKeyCallback(GLFWwindow* window, GLFWkeyfun callback) {}
void glfwSetCursorPosCallback(GLFWwindow* window, GLFWcursorposfun callback) {}
void glfwSetScrollCallback(GLFWwindow* window, GLFWscrollfun callback) {}
void glfwSetFramebufferSizeCallback(GLFWwindow* window, GLFWframebuffersizefun callback) {}
void glfwWindowHint(int hint, int value) {}

#endif
