// Implementation of 3D mesh and geometry creation
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "mesh.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Mesh::Mesh() : VAO_(0), VBO_(0), EBO_(0), index_count_(0), created_(false) {
}

Mesh::~Mesh() {
    Destroy();
}

void Mesh::Create() {
    if (created_) return;
    
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);
    
    created_ = true;
}

void Mesh::Render() {
    if (!created_ || index_count_ == 0) return;
    
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index_count_), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::RenderWireframe() {
    if (!created_ || index_count_ == 0) return;
    
    glBindVertexArray(VAO_);
    glDrawElements(GL_LINES, static_cast<GLsizei>(index_count_), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Destroy() {
    if (created_) {
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);
        created_ = false;
    }
}

void Mesh::SetVertices(const std::vector<float>& vertices) {
    if (!created_) Create();
    
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Mesh::CreateCube() {
    // Cube vertices (8 vertices, 3 coordinates each)
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,  // 0
         0.5f, -0.5f,  0.5f,  // 1
         0.5f,  0.5f,  0.5f,  // 2
        -0.5f,  0.5f,  0.5f,  // 3
        
        // Back face
        -0.5f, -0.5f, -0.5f,  // 4
         0.5f, -0.5f, -0.5f,  // 5
         0.5f,  0.5f, -0.5f,  // 6
        -0.5f,  0.5f, -0.5f   // 7
    };
    
    // Cube indices (12 triangles, 36 indices)
    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        4, 5, 6,  6, 7, 4,
        // Left face
        7, 3, 0,  0, 4, 7,
        // Right face
        1, 5, 6,  6, 2, 1,
        // Top face
        3, 2, 6,  6, 7, 3,
        // Bottom face
        0, 1, 5,  5, 4, 0
    };
    
    SetVertices(vertices);
    SetIndices(indices);
}

void Mesh::CreateCubeWireframe() {
    // Cube vertices (8 vertices, 3 coordinates each)
    std::vector<float> vertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,  // 0
         0.5f, -0.5f,  0.5f,  // 1
         0.5f,  0.5f,  0.5f,  // 2
        -0.5f,  0.5f,  0.5f,  // 3
        
        // Back face
        -0.5f, -0.5f, -0.5f,  // 4
         0.5f, -0.5f, -0.5f,  // 5
         0.5f,  0.5f, -0.5f,  // 6
        -0.5f,  0.5f, -0.5f   // 7
    };
    
    // Wireframe indices (12 edges, 24 indices)
    std::vector<unsigned int> indices = {
        // Front face edges
        0, 1,  1, 2,  2, 3,  3, 0,
        // Back face edges
        4, 5,  5, 6,  6, 7,  7, 4,
        // Connecting edges
        0, 4,  1, 5,  2, 6,  3, 7
    };
    
    SetVertices(vertices);
    SetIndices(indices);
}

void Mesh::SetIndices(const std::vector<unsigned int>& indices) {
    if (!created_) Create();
    
    index_count_ = indices.size();
    
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void Mesh::CreateDisc(float radius, int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    float inner_radius = radius * 0.6f; // Inner radius for hollow disc
    
    // Create vertices for outer circle
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        
        vertices.push_back(x);
        vertices.push_back(0.0f); // y (disc is flat)
        vertices.push_back(z);
    }
    
    // Create vertices for inner circle
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = inner_radius * cos(angle);
        float z = inner_radius * sin(angle);
        
        vertices.push_back(x);
        vertices.push_back(0.0f); // y (disc is flat)
        vertices.push_back(z);
    }
    
    // Create indices for thick ring (wireframe style)
    for (int i = 0; i < segments; ++i) {
        int next_i = (i + 1) % (segments + 1);
        
        // Outer circle lines
        indices.push_back(i);
        indices.push_back(next_i);
        
        // Inner circle lines
        indices.push_back(i + segments + 1);
        indices.push_back(next_i + segments + 1);
        
        // No connecting lines - just thick ring outline
    }
    
    SetVertices(vertices);
    SetIndices(indices);
    Create();
}
