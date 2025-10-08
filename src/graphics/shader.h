// OpenGL shader program wrapper
#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    ~Shader();
    
    bool LoadFromFiles(const std::string& vertex_path, const std::string& fragment_path);
    bool LoadFromStrings(const std::string& vertex_source, const std::string& fragment_source);
    
    void Use();
    void Delete();
    
    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const glm::vec3& value);
    void SetUniform(const std::string& name, const glm::mat4& value);
    
private:
    unsigned int program_id_;
    
    unsigned int CompileShader(unsigned int type, const std::string& source);
    std::string ReadFile(const std::string& filepath);
};
