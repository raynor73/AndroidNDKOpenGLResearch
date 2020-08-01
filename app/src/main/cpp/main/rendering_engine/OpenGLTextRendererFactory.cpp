//
// Created by Igor Lapin on 23/07/2020.
//

#include "OpenGLTextRendererFactory.h"
#include "OpenGLFreeTypeTextRendererComponent.h"

std::shared_ptr<GameObjectComponent> OpenGLTextRendererFactory::createTextRenderer(
        std::vector<std::string> layerNames
) {
    return std::make_shared<OpenGLFreeTypeTextRendererComponent>(
            layerNames,
            m_charactersRepository,
            m_verticalQuadBuffersRepository,
            m_texturesRepository,
            m_openGLErrorDetector
    );
}

void OpenGLTextRendererFactory::removeAllCharacters() {
    m_charactersRepository->removeAllCharacters();
}
