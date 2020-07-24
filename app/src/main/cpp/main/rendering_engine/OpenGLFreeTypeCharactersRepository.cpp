//
// Created by Igor Lapin on 20/07/2020.
//

#include <exception>
#include <sstream>
#include <main/L.h>
#include <main/Constants.h>
#include "OpenGLFreeTypeCharactersRepository.h"

OpenGLFreeTypeCharactersRepository::OpenGLFreeTypeCharactersRepository(
        std::shared_ptr<FontDataLoader> fontDataLoader,
        std::shared_ptr<OpenGLTexturesRepository> texturesRepository
) : m_fontDataLoader(std::move(fontDataLoader)),
    m_texturesRepository(std::move(texturesRepository)) {
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
            //L::w(App::Constants::LOG_TAG, ss.str());
            throw std::domain_error(ss.str());
        }

        auto originalBufferSize = face->glyph->bitmap.width * face->glyph->bitmap.rows;
        std::vector<uint8_t> data(originalBufferSize * COLOR_COMPONENTS);
        for (size_t i = 0; i < originalBufferSize; i++) {
            auto colorFactor = float(face->glyph->bitmap.buffer[i]);
            data[i * COLOR_COMPONENTS + 0] = colorFactor;
            data[i * COLOR_COMPONENTS + 1] = colorFactor;
            data[i * COLOR_COMPONENTS + 2] = colorFactor;
            data[i * COLOR_COMPONENTS + 3] = 1;
        }
        std::stringstream nameBuilder;
        nameBuilder << "glyph_" << int(c) << "_" << textAppearance.fontPath() << "_" << textAppearance.textSize();
        auto name = nameBuilder.str();
        m_texturesRepository->createTexture(name, face->glyph->bitmap.width, face->glyph->bitmap.rows, data);

        auto key = std::pair<char, TextAppearance>(c, textAppearance);
        auto character = Character(
                name,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        );
        m_characterMap.insert({ key, character});
    }
}
