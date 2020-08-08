//
// Created by Igor Lapin on 24/07/2020.
//

#include <GLES2/gl2.h>

#include <exception>
#include <sstream>
#include "OpenGLTexturesRepository.h"

const std::vector<float> OpenGLTexturesRepository::AVAILABLE_BITMAP_DENSITIES = {
        1, 2, 3, 4
};

void OpenGLTexturesRepository::createTexture(
        const std::string& name,
        uint_t width,
        uint_t height,
        const std::vector<uint8_t>& data
) {
    createTexture(name, width, height, data, false);
}

void OpenGLTexturesRepository::createDisplayDensityFactorAwareTexture(const std::string& name, const std::string& path) {
    createDisplayDensityFactorAwareTexture(name, path, false);
}

void OpenGLTexturesRepository::createTexture(const std::string& name, const std::string& path) {
    createTexture(name, path, false);
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
    throwIfTextureAlreadyExists(name);

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

void OpenGLTexturesRepository::createDisplayDensityFactorAwareTexture(
        const std::string& name,
        const std::string& path,
        bool isBeingRestored
) {
    throwIfTextureAlreadyExists(name);

    std::stringstream ss;
    ss << buildDensityPathSegment() << path;
    auto bitmapInfo = m_bitmapDataLoader->loadBitmap(ss.str());

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            bitmapInfo.width,
            bitmapInfo.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bitmapInfo.data.data()
    );

    m_textures[name] = TextureInfo { texture, bitmapInfo.width, bitmapInfo.height };

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (!isBeingRestored) {
        m_texturesCreationParams[name] = DisplayDensityFactorAwareTextureFromFileCreationParams { name, path };
    }

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::createDisplayDensityFactorAwareTexture");
}

void OpenGLTexturesRepository::createTexture(const std::string& name, const std::string& path, bool isBeingRestored) {
    throwIfTextureAlreadyExists(name);

    std::stringstream ss;
    ss << "bitmaps/" << path;
    auto bitmapInfo = m_bitmapDataLoader->loadBitmap(ss.str());

    GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            bitmapInfo.width,
            bitmapInfo.height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            bitmapInfo.data.data()
    );

    m_textures[name] = TextureInfo { texture, bitmapInfo.width, bitmapInfo.height };

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    if (!isBeingRestored) {
        m_texturesCreationParams[name] = TextureFromFileCreationParams { name, path };
    }

    m_openGLErrorDetector->checkOpenGLErrors("OpenGLTexturesRepository::createTexture from file");
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
                    textureFromMemoryCreationParams.data,
                    true
            );
        } else if (std::holds_alternative<DisplayDensityFactorAwareTextureFromFileCreationParams>(entry.second)) {
            auto textureCreationParams = std::get<DisplayDensityFactorAwareTextureFromFileCreationParams>(entry.second);
            createDisplayDensityFactorAwareTexture(textureCreationParams.name, textureCreationParams.path, true);
        } else if (std::holds_alternative<TextureFromFileCreationParams>(entry.second)) {
            auto textureCreationParams = std::get<TextureFromFileCreationParams>(entry.second);
            createTexture(textureCreationParams.name, textureCreationParams.path, true);
        }
        /* else if (std::holds_alternative<GlyphTextureCreationParams>(entry.second)) {
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

std::string OpenGLTexturesRepository::buildDensityPathSegment() {
    std::stringstream ss;
    int roundedDensityFactor = std::ceil(m_displayInfo->densityFactor());

    for (auto availableBitmapDensity : AVAILABLE_BITMAP_DENSITIES) {
        if (availableBitmapDensity >= roundedDensityFactor) {
            ss << "bitmaps/" << availableBitmapDensity << "x/";
            return ss.str();
        }
    }

    ss << AVAILABLE_BITMAP_DENSITIES.back() << "x/";
    return ss.str();
}
