//
// Created by Igor Lapin on 03/08/2020.
//

#include <engine_3d/Utils.h>
#include "SceneCloser.h"

using namespace Engine3D::Utils;

SceneCloser::SceneCloser(
        std::shared_ptr<SceneManager> sceneManager,
        const std::shared_ptr<GameObject>& closeButton,
        SceneType parentSceneType
) : m_sceneManager(std::move(sceneManager)),
    m_parentSceneType(parentSceneType)
{
    throwErrorIfNull(closeButton, "SceneCloser: Close button is null");
    m_closeButtonClickDetector = std::static_pointer_cast<ClickDetectorComponent>(
            closeButton->findComponent(ClickDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(m_closeButtonClickDetector, "SceneCloser: Click detector not found");
}

void SceneCloser::update() {
    if (m_closeButtonClickDetector->isClickDetected()) {
        m_sceneManager->requestSceneLoadAndStart(m_parentSceneType);
    }
}
