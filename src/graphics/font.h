#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int texture_id;  // ID текстуры глифа
    glm::ivec2   size;        // Размер глифа
    glm::ivec2   bearing;     // Смещение от базовой линии до верхнего левого угла
    unsigned int advance;     // Горизонтальное расстояние до следующего глифа
};

class Font {
public:
    Font();
    ~Font();

    bool LoadFont(const std::string& font_path, unsigned int font_size = 48);
    void RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color);
    void Shutdown();

    float GetTextWidth(const std::string& text, float scale = 1.0f) const;
    float GetTextHeight(float scale = 1.0f) const;

private:
    FT_Library ft_;
    FT_Face face_;
    std::map<char, Character> characters_;
    unsigned int VAO_, VBO_;
    unsigned int font_size_;
    bool initialized_;

    void SetupRenderData();
};
