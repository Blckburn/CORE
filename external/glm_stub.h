#ifndef GLM_STUB_H
#define GLM_STUB_H

// Simple GLM stub for development
// This provides basic vector and matrix types

#include <cmath>

namespace glm {
    
    // Basic vector types
    struct vec2 {
        float x, y;
        vec2() : x(0), y(0) {}
        vec2(float x, float y) : x(x), y(y) {}
        vec2 operator+(const vec2& other) const { return vec2(x + other.x, y + other.y); }
        vec2 operator-(const vec2& other) const { return vec2(x - other.x, y - other.y); }
        vec2 operator*(float scalar) const { return vec2(x * scalar, y * scalar); }
        float length() const { return std::sqrt(x*x + y*y); }
        vec2 normalize() const { float len = length(); return len > 0 ? *this * (1.0f/len) : vec2(); }
    };
    
    struct vec3 {
        float x, y, z;
        vec3() : x(0), y(0), z(0) {}
        vec3(float x, float y, float z) : x(x), y(y), z(z) {}
        vec3 operator+(const vec3& other) const { return vec3(x + other.x, y + other.y, z + other.z); }
        vec3 operator-(const vec3& other) const { return vec3(x - other.x, y - other.y, z - other.z); }
        vec3 operator*(float scalar) const { return vec3(x * scalar, y * scalar, z * scalar); }
        float length() const { return std::sqrt(x*x + y*y + z*z); }
        vec3 normalize() const { float len = length(); return len > 0 ? *this * (1.0f/len) : vec3(); }
    };
    
    struct vec4 {
        float x, y, z, w;
        vec4() : x(0), y(0), z(0), w(0) {}
        vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        vec4(const vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}
    };
    
    // Basic matrix types
    struct mat4 {
        float m[16];
        mat4() {
            for(int i = 0; i < 16; i++) m[i] = 0;
            m[0] = m[5] = m[10] = m[15] = 1; // Identity
        }
        const float* operator[](int row) const { return &m[row * 4]; }
        float* operator[](int row) { return &m[row * 4]; }
    };
    
    // Basic functions
    inline float radians(float degrees) { return degrees * 3.14159265f / 180.0f; }
    inline vec3 cross(const vec3& a, const vec3& b) {
        return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    }
    inline float dot(const vec3& a, const vec3& b) {
        return a.x*b.x + a.y*b.y + a.z*b.z;
    }
    inline float length(const vec3& v) { return v.length(); }
    inline vec3 normalize(const vec3& v) { return v.normalize(); }
    inline float max(float a, float b) { return a > b ? a : b; }
    inline float min(float a, float b) { return a < b ? a : b; }
    
    // Matrix functions (simplified)
    mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up);
    mat4 perspective(float fov, float aspect, float near, float far);
    mat4 translate(const mat4& m, const vec3& v);
    mat4 scale(const mat4& m, const vec3& v);
    mat4 rotate(const mat4& m, float angle, const vec3& axis);
    
    // Placeholder implementations
    inline mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) {
        mat4 result;
        // Simplified lookAt - just return identity for now
        return result;
    }
    
    inline mat4 perspective(float fov, float aspect, float near, float far) {
        mat4 result;
        // Simplified perspective - just return identity for now
        return result;
    }
    
    inline mat4 translate(const mat4& m, const vec3& v) { return m; }
    inline mat4 scale(const mat4& m, const vec3& v) { return m; }
    inline mat4 rotate(const mat4& m, float angle, const vec3& axis) { return m; }
}

#endif
