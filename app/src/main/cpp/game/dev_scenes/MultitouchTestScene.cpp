//
// Created by Igor Lapin on 03/08/2020.
//

#include <memory>
#include <engine_3d/TextComponent.h>
#include <engine_3d/Utils.h>
#include <engine_3d/TransformationComponent.h>
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

    for (auto& touchEvent : m_rootGestureConsumer->touchEvents()) {
        if (m_activeTouchIndicators.count(touchEvent.id) > 0) {
            auto touchIndicator = m_activeTouchIndicators.at(touchEvent.id);
            if (touchEvent.type == TouchEventType::UP || touchEvent.type == TouchEventType::CANCEL) {
                m_rootGameObject->removeChild(touchIndicator);
                m_touchIndicatorsPool.recycle(touchIndicator);
                m_activeTouchIndicators.erase(touchEvent.id);
            } else {
                auto transform = std::static_pointer_cast<TransformationComponent>(
                        touchIndicator->findComponent(TransformationComponent::TYPE_NAME)
                );
                auto position = transform->position();
                position.x = touchEvent.x;
                position.y = touchEvent.y;
                transform->setPosition(position);
            }
        } else {
            if (touchEvent.type == TouchEventType::DOWN || touchEvent.type == TouchEventType::MOVE) {
                auto touchIndicator = m_touchIndicatorsPool.obtain();

                m_rootGameObject->addChild(touchIndicator);
                m_activeTouchIndicators[touchEvent.id] = touchIndicator;

                auto transform = std::static_pointer_cast<TransformationComponent>(
                        touchIndicator->findComponent(TransformationComponent::TYPE_NAME)
                );
                auto position = transform->position();
                position.x = touchEvent.x;
                position.y = touchEvent.y;
                transform->setPosition(position);
            }
        }
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
