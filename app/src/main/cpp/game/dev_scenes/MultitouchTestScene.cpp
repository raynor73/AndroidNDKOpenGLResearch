//
// Created by Igor Lapin on 03/08/2020.
//

#include <memory>
#include <engine_3d/TextComponent.h>
#include <engine_3d/Utils.h>
#include "MultitouchTestScene.h"

using namespace Engine3D::Utils;

MultitouchTestScene::MultitouchTestScene(
        std::shared_ptr<TimeProvider> timeProvider,
        std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<UnitsConverter> unitsConverter,
        std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
        std::shared_ptr<MeshRendererFactory> meshRendererFactory,
        std::shared_ptr<TextRendererFactory> textRendererFactory,
        std::shared_ptr<TouchScreen> touchScreen,
        std::shared_ptr<TexturesRepository> texturesRepository,
        std::shared_ptr<SceneManager> sceneManager
) : Scene(
        std::move(timeProvider),
        std::move(displayInfo),
        std::move(unitsConverter),
        std::move(meshLoadingRepository),
        std::move(meshRendererFactory),
        std::move(textRendererFactory),
        std::move(touchScreen),
        std::move(texturesRepository)
),
    m_sceneManager(std::move(sceneManager)),
    m_touchIndicatorsPool([&]() {
        return m_touchIndicatorPrefab->clone();
    }) {}

void MultitouchTestScene::update(float dt) {
    m_fpsCalculator.update(dt);

    if (m_sceneCloser != nullptr) {
        m_sceneCloser->update();
    }

    auto fpsText = std::static_pointer_cast<TextComponent>(
            m_gameObjectsMap["fpsText"]->findComponent(TextComponent::TYPE_NAME)
    );

    {
        std::stringstream ss;
        ss  << "FPS: " << int(m_fpsCalculator.fps());
        fpsText->setText(ss.str());
    }
}

void MultitouchTestScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    Scene::restoreFromStateRepresentation(stateRepresentation);

    m_sceneCloser = std::make_shared<SceneCloser>(
            m_sceneManager,
            m_gameObjectsMap.at("closeButton"),
            SceneType::SCENES_SELECTION_SCENE
    );

    m_touchIndicatorPrefab = m_gameObjectsMap.at("touchIndicatorPrefab");
    m_rootGameObject->removeChild(m_touchIndicatorPrefab);

    m_rootGestureConsumer = std::static_pointer_cast<GestureConsumerComponent>(
            m_rootGameObject->findComponent(GestureConsumerComponent::TYPE_NAME)
    );
    throwErrorIfNull(m_rootGestureConsumer, "Root Gesture Consumer not found");
}
