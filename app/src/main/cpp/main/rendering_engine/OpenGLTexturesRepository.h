//
// Created by Igor Lapin on 24/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_TEXTURES_REPOSITORY_H
#define RENDERING_ENGINE_OPENGL_TEXTURES_REPOSITORY_H


#include <memory>
#include <vector>
#include <unordered_map>
#include <variant>
#include <optional>
#include <game/TexturesRepository.h>
#include <main/OpenGLErrorDetector.h>
#include "TextureCreationParams.h"
#include "TextureInfo.h"

class OpenGLTexturesRepository : public TexturesRepository {

    std::unordered_map<std::string, TextureInfo> m_textures;

    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

    std::unordered_map<std::string, std::variant<TextureFromMemoryCreationParams/*, GlyphTextureCreationParams*/>> m_texturesCreationParams;

public:
    OpenGLTexturesRepository(
            std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector
    ) : m_openGLErrorDetector(openGLErrorDetector) {}
    OpenGLTexturesRepository(const OpenGLTexturesRepository&) = delete;
    OpenGLTexturesRepository(OpenGLTexturesRepository&&) = delete;

    virtual void createTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data
    ) override;

    /*virtual void createGlyphTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data
    ) override;*/

    std::optional<TextureInfo> findTexture(const std::string& name);

    void restoreTextures();

    void removeTexture(const std::string& name);

    void removeAllTextures();

    OpenGLTexturesRepository& operator=(const OpenGLTexturesRepository&) = delete;
    OpenGLTexturesRepository& operator=(OpenGLTexturesRepository&&) = delete;

private:
    void createTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data,
            bool isBeingRestored
    );

    /*void createGlyphTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data,
            bool isBeingRestored
    );*/
};


#endif //RENDERING_ENGINE_OPENGL_TEXTURES_REPOSITORY_H
