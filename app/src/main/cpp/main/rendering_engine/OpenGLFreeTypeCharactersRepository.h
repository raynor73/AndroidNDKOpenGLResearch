//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H
#define RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H


#include <ft2build.h>
#include FT_FREETYPE_H
#include <unordered_map>
#include <memory>
#include <game/CharactersRepository.h>
#include <game/FontDataLoader.h>
#include <game/UnitsConverter.h>
#include <game/DisplayInfoUpdateDetector.h>
#include "OpenGLTexturesRepository.h"

namespace std {

    template<> struct hash<ComplexValue> {
        size_t operator()(const ComplexValue& complexValue) const {
            size_t typeHash;
            size_t valueHash;
            if (std::holds_alternative<PercentValue>(complexValue)) {
                auto percentValue = std::get<PercentValue>(complexValue);

                typeHash = std::hash<std::string>{}("%");
                valueHash = std::hash<float>{}(percentValue.value);
            } else if (std::holds_alternative<DpValue>(complexValue)) {
                auto dpValue = std::get<DpValue>(complexValue);

                typeHash = std::hash<std::string>{}("dp");
                valueHash = std::hash<float>{}(dpValue.value);
            } else {
                auto plainValue = std::get<PlainValue>(complexValue);

                typeHash = std::hash<std::string>{}("px");
                valueHash = std::hash<float>{}(plainValue.value);
            }

            return typeHash ^ (valueHash << 1);
        }
    };

    template<> struct hash<TextAppearance> {
        size_t operator()(const TextAppearance& textAppearance) const {
            auto textSizeHash = std::hash<ComplexValue>{}(textAppearance.textSize());
            auto fontPathHash = std::hash<std::string>{}(textAppearance.fontPath());
            return textSizeHash ^ (fontPathHash << 1);
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

class OpenGLFreeTypeCharactersRepository : public CharactersRepository, DisplayInfoUpdateDetector {

    FT_Library m_freeType;
    std::unordered_map<std::pair<char, TextAppearance>, Character> m_characterMap;
    std::shared_ptr<FontDataLoader> m_fontDataLoader;
    std::shared_ptr<OpenGLTexturesRepository> m_texturesRepository;
    std::shared_ptr<UnitsConverter> m_unitsConverter;

public:
    OpenGLFreeTypeCharactersRepository(
            std::shared_ptr<FontDataLoader> fontDataLoader,
            std::shared_ptr<OpenGLTexturesRepository> texturesRepository,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<DisplayInfo> displayInfo
    );
    OpenGLFreeTypeCharactersRepository(const OpenGLFreeTypeCharactersRepository&) = delete;
    OpenGLFreeTypeCharactersRepository(OpenGLFreeTypeCharactersRepository&&) = delete;
    virtual ~OpenGLFreeTypeCharactersRepository();

    virtual const Character& getCharacter(const TextAppearance& textAppearance, char c) override;

    void createCharacters(const TextAppearance& textAppearance);
    void removeAllCharacters();

    OpenGLFreeTypeCharactersRepository& operator=(const OpenGLFreeTypeCharactersRepository&) = delete;
    OpenGLFreeTypeCharactersRepository& operator=(const OpenGLFreeTypeCharactersRepository&&) = delete;

private:
    static const int COLOR_COMPONENTS = 4;
};


#endif //RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H
