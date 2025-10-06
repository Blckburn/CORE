#include "glad.h"
#include <GLFW/glfw3.h>

// Simple GLAD implementation for OpenGL 3.3
// This is a minimal implementation for development

int gladLoadGLLoader(GLADloadproc load) {
    // For now, just return success
    // In a real implementation, this would load OpenGL functions
    return 1;
}

