//
// Created by Igor Lapin on 24/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_TEXTURES_REPOSITORY_H
#define RENDERING_ENGINE_OPENGL_TEXTURES_REPOSITORY_H


#include <memory>
#include <vector>
#include <unordered_map>
#include <game/TexturesRepository.h>
#include <main/OpenGLErrorDetector.h>
#include "TextureCreationParams.h"

class OpenGLTexturesRepository : public TexturesRepository {

    std::unordered_map<std::string, GLuint> m_textures;

    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;

    std::vector<std::variant<VertexBufferCreationParams, IndexBufferCreationParams>> m_buffersCreationParams;

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
};


#endif //RENDERING_ENGINE_OPENGL_TEXTURES_REPOSITORY_H
