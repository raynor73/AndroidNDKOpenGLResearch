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
#include <main/AndroidBitmapDataLoader.h>
#include <game/DisplayInfo.h>
#include "TextureCreationParams.h"
#include "TextureInfo.h"

class OpenGLTexturesRepository : public TexturesRepository {

    std::unordered_map<std::string, TextureInfo> m_textures;

    std::shared_ptr<DisplayInfo> m_displayInfo;
    std::shared_ptr<BitmapDataLoader> m_bitmapDataLoader;
    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

    std::unordered_map<std::string, std::variant<TextureFromMemoryCreationParams/*, GlyphTextureCreationParams*/, DisplayDensityFactorAwareTextureFromFileCreationParams>> m_texturesCreationParams;

public:
    OpenGLTexturesRepository(
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<BitmapDataLoader> bitmapDataLoader,
            std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector
    ) : m_displayInfo(displayInfo),
        m_bitmapDataLoader(bitmapDataLoader),
        m_openGLErrorDetector(openGLErrorDetector) {}
    OpenGLTexturesRepository(const OpenGLTexturesRepository&) = delete;
    OpenGLTexturesRepository(OpenGLTexturesRepository&&) = delete;

    virtual void createTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data
    ) override;

    virtual void createDisplayDensityFactorAwareTexture(const std::string& name, const std::string& path) override;

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
    inline void throwIfTextureAlreadyExists(const std::string& name) {
        if (m_textures.count(name) > 0) {
            std::stringstream ss;
            ss << "Texture " << name << " already exists";
            throw std::domain_error(ss.str());
        }
    }

    void createTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data,
            bool isBeingRestored
    );

    void createDisplayDensityFactorAwareTexture(const std::string& name, const std::string& path, bool isBeingRestored);

    /*void createGlyphTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data,
            bool isBeingRestored
    );*/

    std::string buildDensityPathSegment();

    static const std::vector<float> AVAILABLE_BITMAP_DENSITIES;
};


#endif //RENDERING_ENGINE_OPENGL_TEXTURES_REPOSITORY_H
