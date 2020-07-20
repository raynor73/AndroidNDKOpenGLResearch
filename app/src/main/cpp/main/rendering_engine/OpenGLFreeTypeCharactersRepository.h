//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H
#define RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H


#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <game/CharactersRepository.h>

namespace std {
    template<> struct hash<TextAppearance> {
        size_t operator()(const TextAppearance& textAppearance) const {
            auto textSizeHash = std::hash<unsigned int>{}(textAppearance.textSize());
            auto fontNameHash = std::hash<std::string>{}(textAppearance.fontName());
            return textSizeHash ^ (fontNameHash << 1);
        }
    };

    template<> struct hash<std::pair<char, TextAppearance>> {
        size_t operator()(const std::pair<char, TextAppearance>& charWithTextAppearance) const {
            auto charHash = std::hash<char>{}(charWithTextAppearance.first);
            auto textAppearanceHash = std::hash<TextAppearance>{}(charWithTextAppearance.second);
            return charHash ^ (textAppearanceHash << 1);
        }
    };
}

class OpenGLFreeTypeCharactersRepository : public CharactersRepository {

    FT_Library m_freeType;
    std::unordered_map<std::pair<char, TextAppearance>, Character> m_characterMap;

public:
    OpenGLFreeTypeCharactersRepository();
    OpenGLFreeTypeCharactersRepository(const OpenGLFreeTypeCharactersRepository&) = delete;
    OpenGLFreeTypeCharactersRepository(OpenGLFreeTypeCharactersRepository&&) = delete;
    virtual ~OpenGLFreeTypeCharactersRepository();

    virtual const Character& getCharacter(const TextAppearance& textAppearance, char c) override;

    void createCharacters(const TextAppearance& textAppearance);

    OpenGLFreeTypeCharactersRepository& operator=(const OpenGLFreeTypeCharactersRepository&) = delete;
    OpenGLFreeTypeCharactersRepository& operator=(const OpenGLFreeTypeCharactersRepository&&) = delete;
};


#endif //RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H
