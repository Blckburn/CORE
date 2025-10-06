#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : program_id_(0) {
}

Shader::~Shader() {
    Delete();
}

bool Shader::LoadFromFiles(const std::string& vertex_path, const std::string& fragment_path) {
    std::string vertex_source = ReadFile(vertex_path);
    std::string fragment_source = ReadFile(fragment_path);
    
    if (vertex_source.empty() || fragment_source.empty()) {
        return false;
    }
    
    return LoadFromStrings(vertex_source, fragment_source);
}

bool Shader::LoadFromStrings(const std::string& vertex_source, const std::string& fragment_source) {
    unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_source);
    if (vertex_shader == 0) return false;
    
    unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_source);
    if (fragment_shader == 0) {
        glDeleteShader(vertex_shader);
        return false;
    }
    
    program_id_ = glCreateProgram();
    glAttachShader(program_id_, vertex_shader);
    glAttachShader(program_id_, fragment_shader);
    glLinkProgram(program_id_);
    
    // Check for linking errors
    int success;
    glGetProgramiv(program_id_, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetProgramInfoLog(program_id_, 512, nullptr, info_log);
        std::cerr << "Shader linking failed: " << info_log << std::endl;
        
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(program_id_);
        program_id_ = 0;
        return false;
    }
    
    // Clean up shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return true;
}

void Shader::Use() {
    if (program_id_ != 0) {
        glUseProgram(program_id_);
    }
}

void Shader::Delete() {
    if (program_id_ != 0) {
        glDeleteProgram(program_id_);
        program_id_ = 0;
    }
}

void Shader::SetUniform(const std::string& name, bool value) {
    int location = glGetUniformLocation(program_id_, name.c_str());
    if (location != -1) {
        glUniform1i(location, value ? 1 : 0);
    }
}

void Shader::SetUniform(const std::string& name, int value) {
    int location = glGetUniformLocation(program_id_, name.c_str());
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::SetUniform(const std::string& name, float value) {
    int location = glGetUniformLocation(program_id_, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::SetUniform(const std::string& name, const glm::vec3& value) {
    int location = glGetUniformLocation(program_id_, name.c_str());
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void Shader::SetUniform(const std::string& name, const glm::mat4& value) {
    int location = glGetUniformLocation(program_id_, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    // Check for compilation errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        std::cerr << "Shader compilation failed: " << info_log << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    
    return shader;
}

std::string Shader::ReadFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
