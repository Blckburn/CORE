#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Mesh {
public:
    Mesh();
    ~Mesh();
    
    void Create();
    void Render();
    void RenderWireframe();
    void Destroy();
    
    void SetVertices(const std::vector<float>& vertices);
    void SetIndices(const std::vector<unsigned int>& indices);
    void CreateCube();
    void CreateCubeWireframe();
    void CreateDisc(float radius = 0.2f, int segments = 16);
    
private:
    GLuint VAO_, VBO_, EBO_;
    size_t index_count_;
    bool created_;
};
