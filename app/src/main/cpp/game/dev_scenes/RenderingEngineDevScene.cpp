//
// Created by Igor Lapin on 28/06/2020.
//

#include <sstream>
#include <glm/gtc/quaternion.hpp>
#include <engine_3d/TextComponent.h>
#include "RenderingEngineDevScene.h"
#include <main/L.h>

using namespace Engine3D::Utils;

RenderingEngineDevScene::RenderingEngineDevScene(
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
        timeProvider,
        displayInfo,
        unitsConverter,
        meshLoadingRepository,
        meshRendererFactory,
        textRendererFactory,
        touchScreen,
        texturesRepository
    ), m_sceneManager(std::move(sceneManager))
{
    m_cameraRotationSensitivity = 90 / m_displayInfo->width();
}

void RenderingEngineDevScene::update(float dt) {
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

    m_boxAngleX += dt * 45;
    m_boxAngleY += dt * 45;
    m_boxAngleZ += dt * 45;

    m_box2AngleX += dt * 30;
    m_box2AngleY += dt * 30;
    m_box2AngleZ += dt * 30;

    auto rotationMatrix = glm::identity<glm::mat4>();
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_boxAngleZ), glm::vec3(0, 0, 1));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_boxAngleX), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_boxAngleY), glm::vec3(0, 1, 0));
    auto rotationQuaternion = glm::quat_cast(rotationMatrix);
    m_boxTransform->setRotation(rotationQuaternion);

    rotationMatrix = glm::identity<glm::mat4>();
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_box2AngleZ), glm::vec3(0, 0, 1));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_box2AngleX), glm::vec3(1, 0, 0));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(m_box2AngleY), glm::vec3(0, 1, 0));
    rotationQuaternion = glm::quat_cast(rotationMatrix);
    m_box2Transform->setRotation(rotationQuaternion);

    auto optionalScrollEvent = m_rightControllerAreaScrollDetector->scrollEvent();
    if (optionalScrollEvent) {
        auto scrollEvent = optionalScrollEvent.value();

        m_cameraAngleX += glm::radians(scrollEvent.dy* m_cameraRotationSensitivity);
        m_cameraAngleY += glm::radians(-scrollEvent.dx * m_cameraRotationSensitivity);

        auto rotation = glm::rotate(
                glm::identity<glm::quat>(),
                m_cameraAngleY,
                glm::vec3(0, 1, 0)
        );
        rotation = glm::rotate(
                rotation,
                m_cameraAngleX,
                glm::vec3(1, 0, 0)
        );
        m_cameraTransform->setRotation(rotation);
    }
}

void RenderingEngineDevScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    Scene::restoreFromStateRepresentation(stateRepresentation);

    m_sceneCloser = std::make_shared<SceneCloser>(
            m_sceneManager,
            m_gameObjectsMap.at("closeButton"),
            SceneType::SCENES_SELECTION_SCENE
    );

    auto boxGameObject = m_gameObjectsMap.at("box");
    throwErrorIfNull(boxGameObject, "Box not found");
    m_boxTransform = std::static_pointer_cast<TransformationComponent>(boxGameObject->findComponent(
            TransformationComponent::TYPE_NAME
    ));
    throwErrorIfNull(m_boxTransform, "Box has not transform");

    auto box2GameObject = m_gameObjectsMap.at("box2");
    throwErrorIfNull(box2GameObject, "Box2 not found");
    m_box2Transform = std::static_pointer_cast<TransformationComponent>(box2GameObject->findComponent(
            TransformationComponent::TYPE_NAME
    ));
    throwErrorIfNull(m_box2Transform, "Box has not transform");

    auto rightControllerAreaGameObject = m_gameObjectsMap.at("rightControllerArea");
    throwErrorIfNull(rightControllerAreaGameObject, "No right controller area game object");
    m_rightControllerAreaScrollDetector = std::static_pointer_cast<ScrollDetectorComponent>(
            rightControllerAreaGameObject->findComponent(ScrollDetectorComponent::TYPE_NAME)
    );
    throwErrorIfNull(rightControllerAreaGameObject, "No right controller area scroll detector");

    m_cameraTransform = findComponent<TransformationComponent>("sceneCamera", TransformationComponent::TYPE_NAME);
}
