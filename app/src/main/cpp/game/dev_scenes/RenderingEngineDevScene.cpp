//
// Created by Igor Lapin on 28/06/2020.
//

#include <engine_3d/TextComponent.h>
#include "RenderingEngineDevScene.h"

void RenderingEngineDevScene::update(float dt) {
    m_fpsCalculator.update(dt);

    auto fpsText = std::static_pointer_cast<TextComponent>(
            m_gameObjectsMap["fpsText"]->findComponent(TextComponent::TYPE_NAME)
    );

    {
        std::stringstream ss;
        ss  << "FPS: " << int(m_fpsCalculator.fps());
        fpsText->setText(ss.str());
    }
}
