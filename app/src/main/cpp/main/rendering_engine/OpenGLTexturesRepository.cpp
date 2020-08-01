//
// Created by Igor Lapin on 24/07/2020.
//

#include <GLES2/gl2.h>

#include <exception>
#include <sstream>
#include "OpenGLTexturesRepository.h"

void OpenGLTexturesRepository::createTexture(
        const std::string& name,
        uint_t width,
        uint_t height,
        const std::vector<uint8_t>& data
) {
    createTexture(name, width, height, data, false);
}

/*void OpenGLTexturesRepository::createGlyphTexture(
        const std::string& name,
        uint_t width,
        uint_t height,
        const std::vector<uint8_t>& data
) {
    createGlyphTexture(name, width, height, data, false);
}*/

void OpenGLTexturesRepository::createTexture(
        const std::string& name,
        uint_t width,
        uint_t height,
        const std::vector<uint8_t>& data,
        bool isBeingRestored
) {
    if (m_textures.count(name) > 0) {
        std::stringstream ss;
        ss << "Texture " << name << " already exists";
        throw std::domain_error(ss.str());
    }

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    m_textures[name] = TextureInfo { texture, width, height };

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (!isBeingRestored) {
        m_texturesCreationParams[name] = TextureFromMemoryCreationParams { name, width, height, data };
    }

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::createTexture from memory");
}

/*void OpenGLTexturesRepository::createGlyphTexture(
        const std::string& name,
        uint_t width,
        uint_t height,
        const std::vector<uint8_t>& data,
        bool isBeingRestored
) {
    if (m_textures.count(name) > 0) {
        std::stringstream ss;
        ss << "Texture " << name << " already exists";
        throw std::domain_error(ss.str());
    }

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data.data());

    m_textures[name] = TextureInfo { texture, width, height };

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (!isBeingRestored) {
        m_texturesCreationParams[name] = GlyphTextureCreationParams { name, width, height, data };
    }

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::createGlyphTexture");
}*/

std::optional<TextureInfo> OpenGLTexturesRepository::findTexture(const std::string& name) {
    if (m_textures.count(name) > 0) {
        return m_textures[name];
    } else {
        return std::optional<TextureInfo>();
    }
}

void OpenGLTexturesRepository::removeTexture(const std::string& name) {
    if (m_textures.count(name) == 0) {
        std::stringstream ss;
        ss << "No texture " << name << " to remove";
        throw std::domain_error(ss.str());
    }

    auto textureInfo = m_textures[name];
    glDeleteTextures(1, &textureInfo.texture);
    m_textures.erase(name);

    m_texturesCreationParams.erase(name);

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::removeTexture");
}

void OpenGLTexturesRepository::removeAllTextures() {
    std::vector<std::string> namesToDelete;

    for (auto& entry : m_textures) {
        namesToDelete.push_back(entry.first);
    }
    for (auto& name : namesToDelete) {
        removeTexture(name);
    }
}

void OpenGLTexturesRepository::restoreTextures() {
    m_textures.clear();
    for (auto& entry : m_texturesCreationParams) {
        if (std::holds_alternative<TextureFromMemoryCreationParams>(entry.second)) {
            auto textureFromMemoryCreationParams = std::get<TextureFromMemoryCreationParams>(entry.second);
            createTexture(
                    textureFromMemoryCreationParams.name,
                    textureFromMemoryCreationParams.width,
                    textureFromMemoryCreationParams.height,
                    textureFromMemoryCreationParams.data
            );
        }

        /*if (std::holds_alternative<GlyphTextureCreationParams>(entry.second)) {
            auto glyphTextureCreationParams = std::get<GlyphTextureCreationParams>(entry.second);
            createGlyphTexture(
                    glyphTextureCreationParams.name,
                    glyphTextureCreationParams.width,
                    glyphTextureCreationParams.height,
                    glyphTextureCreationParams.data
            );
        }*/
    }
}
