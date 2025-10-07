#include "font.h"
#include <iostream>
#include <glad/glad.h>

Font::Font() : initialized_(false), font_size_(48) {
    VAO_ = 0;
    VBO_ = 0;
}

Font::~Font() {
    Shutdown();
}

bool Font::LoadFont(const std::string& font_path, unsigned int font_size) {
    font_size_ = font_size;
    
    // Инициализация FreeType
    if (FT_Init_FreeType(&ft_)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }
    std::cout << "FreeType initialized successfully" << std::endl;

    // Загрузка шрифта
    if (FT_New_Face(ft_, font_path.c_str(), 0, &face_)) {
        std::cout << "ERROR::FREETYPE: Failed to load font from path: " << font_path << std::endl;
        FT_Done_FreeType(ft_);
        return false;
    }

    // Установка размера шрифта
    FT_Set_Pixel_Sizes(face_, 0, font_size);

    // Отключение байт-выравнивания
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Загрузка первых 128 символов ASCII
    for (unsigned char c = 0; c < 128; c++) {
        // Загрузка глифа символа
        if (FT_Load_Char(face_, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        // Генерация текстуры
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face_->glyph->bitmap.width,
            face_->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face_->glyph->bitmap.buffer
        );

        // Установка параметров текстуры
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Сохранение символа для последующего использования
        Character character = {
            texture,
            glm::ivec2(face_->glyph->bitmap.width, face_->glyph->bitmap.rows),
            glm::ivec2(face_->glyph->bitmap_left, face_->glyph->bitmap_top),
            static_cast<unsigned int>(face_->glyph->advance.x)
        };
        characters_.insert(std::pair<char, Character>(c, character));
    }

    // Освобождение ресурсов FreeType
    FT_Done_Face(face_);
    FT_Done_FreeType(ft_);

    // Настройка данных для рендеринга
    SetupRenderData();

    initialized_ = true;
    std::cout << "Font loaded successfully: " << font_path << std::endl;
    return true;
}

void Font::SetupRenderData() {
    // Настройка VAO/VBO для рендеринга текста
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glBindVertexArray(VAO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Font::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color) {
    if (!initialized_) return;

    // Включение смешивания для прозрачности
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Активация соответствующих состояний
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO_);

    // Итерация по всем символам
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters_[*c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        // Обновление VBO для текущего символа
        // Flip V texcoord to account for FreeType's top-left origin
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos,     ypos,       0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 0.0f },

            { xpos,     ypos + h,   0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 1.0f }
        };

        // Рендеринг глифа текстуры на квадрате
        glBindTexture(GL_TEXTURE_2D, ch.texture_id);
        
        // Обновление содержимого VBO памяти
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        // Рендеринг квадрата
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // Теперь продвигаем курсоры для следующего глифа (обратите внимание, что advance - это число 1/64 пикселя)
        x += (ch.advance >> 6) * scale; // биты сдвига на 6, чтобы получить значение в пикселях (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Отключение смешивания
    glDisable(GL_BLEND);
}

float Font::GetTextWidth(const std::string& text, float scale) const {
    float width = 0.0f;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = characters_.at(*c);
        width += (ch.advance >> 6) * scale;
    }
    return width;
}

float Font::GetTextHeight(float scale) const {
    // Возвращаем высоту строки шрифта
    return font_size_ * scale;
}

void Font::Shutdown() {
    if (initialized_) {
        // Очистка текстур
        for (auto& pair : characters_) {
            glDeleteTextures(1, &pair.second.texture_id);
        }
        
        if (VAO_) {
            glDeleteVertexArrays(1, &VAO_);
        }
        if (VBO_) {
            glDeleteBuffers(1, &VBO_);
        }
        
        initialized_ = false;
    }
}
