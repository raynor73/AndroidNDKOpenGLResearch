//
// Created by Igor Lapin on 23/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_TEXT_RENDERER_FACTORY_H
#define RENDERING_ENGINE_OPENGL_TEXT_RENDERER_FACTORY_H


#include <engine_3d/TextRendererFactory.h>
#include <main/OpenGLErrorDetector.h>
#include "OpenGLGeometryBuffersStorage.h"
#include "OpenGLVerticalQuadBuffersRepository.h"
#include "OpenGLFreeTypeCharactersRepository.h"
#include "OpenGLTexturesRepository.h"

class OpenGLTextRendererFactory : public TextRendererFactory {

    std::shared_ptr<OpenGLErrorDetector> m_openGLErrorDetector;
    std::shared_ptr<OpenGLVerticalQuadBuffersRepository> m_verticalQuadBuffersRepository;
    std::shared_ptr<OpenGLFreeTypeCharactersRepository> m_charactersRepository;
    std::shared_ptr<OpenGLTexturesRepository> m_texturesRepository;

public:
    OpenGLTextRendererFactory(
            std::shared_ptr<OpenGLErrorDetector> openGLErrorDetector,
            std::shared_ptr<OpenGLVerticalQuadBuffersRepository> verticalQuadBuffersRepository,
            std::shared_ptr<OpenGLFreeTypeCharactersRepository> charactersRepository,
            std::shared_ptr<OpenGLTexturesRepository> texturesRepository
    ) : m_openGLErrorDetector(openGLErrorDetector),
        m_verticalQuadBuffersRepository(verticalQuadBuffersRepository),
        m_charactersRepository(charactersRepository),
        m_texturesRepository(texturesRepository) {}
    OpenGLTextRendererFactory(const OpenGLTextRendererFactory&) = delete;
    OpenGLTextRendererFactory(OpenGLTextRendererFactory&&) = delete;

    virtual std::shared_ptr<GameObjectComponent> createTextRenderer(std::vector<std::string> layerNames) override;
    void removeAllCharacters();

    OpenGLTextRendererFactory& operator=(const OpenGLTextRendererFactory&) = delete;
    OpenGLTextRendererFactory& operator=(OpenGLTextRendererFactory&&) = delete;
};


#endif //RENDERING_ENGINE_OPENGL_TEXT_RENDERER_FACTORY_H
