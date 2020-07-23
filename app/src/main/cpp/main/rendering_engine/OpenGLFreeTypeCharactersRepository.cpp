//
// Created by Igor Lapin on 20/07/2020.
//

#include <exception>
#include <sstream>
#include <main/L.h>
#include <main/Constants.h>
#include "OpenGLFreeTypeCharactersRepository.h"

OpenGLFreeTypeCharactersRepository::OpenGLFreeTypeCharactersRepository(
        std::shared_ptr<FontDataLoader> fontDataLoader
) : m_fontDataLoader(fontDataLoader) {
    if (FT_Init_FreeType(&m_freeType)){
        throw std::domain_error("Could not init FreeType library");
    }
}

OpenGLFreeTypeCharactersRepository::~OpenGLFreeTypeCharactersRepository() {
    FT_Done_FreeType(m_freeType);
}

const Character& OpenGLFreeTypeCharactersRepository::getCharacter(const TextAppearance& textAppearance, char c) {
    auto key = std::pair<char, TextAppearance>(c, textAppearance);
    if (m_characterMap.count(key) == 0) {
        createCharacters(textAppearance);
    }
    if (m_characterMap.count(key) == 0) {
        std::stringstream ss;
        ss <<
            "Character '" <<
            c <<
            "' of size " <<
            textAppearance.textSize() <<
            " and font path " <<
            textAppearance.fontPath() <<
            " not found";
        throw std::domain_error(ss.str());
    }

    return m_characterMap.at(key);
}

void OpenGLFreeTypeCharactersRepository::createCharacters(const TextAppearance& textAppearance) {
    auto fontData = m_fontDataLoader->loadFontData(textAppearance.fontPath());
    FT_Face face;
    if (FT_New_Memory_Face(m_freeType, fontData.data(), fontData.size(), 0, &face)) {
        std::stringstream ss;
        ss << "Failed to load font " << textAppearance.fontPath();
        throw std::domain_error(ss.str());
    }

    FT_Set_Pixel_Sizes(face, 0, textAppearance.textSize());

    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::stringstream ss;
            ss << "Failed to load glyph '" << c << "'";
            L::w(App::Constants::LOG_TAG, ss.str());
            //throw std::domain_error(ss.str());
        }
        /*// generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));*/
        auto key = std::pair<char, TextAppearance>(c, textAppearance);
        auto character = Character(
                "",
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        );
        m_characterMap.insert({ key, character});
    }
    /*glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }*/
}
