//
// Created by Igor Lapin on 28/06/2020.
//

#include <sstream>
#include <glm/gtc/quaternion.hpp>
#include <engine_3d/TextComponent.h>
#include <main/L.h>
#include <engine_3d/Constants.h>
#include <engine_3d/CollisionsInfoComponent.h>
#include <engine_3d/RigidBodyComponent.h>
#include "RenderingEngineDevScene.h"

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
        std::shared_ptr<SceneManager> sceneManager,
        std::shared_ptr<PhysicsEngine> physicsEngine
) : Scene(
        std::move(timeProvider),
        std::move(displayInfo),
        std::move(unitsConverter),
        std::move(meshLoadingRepository),
        std::move(meshRendererFactory),
        std::move(textRendererFactory),
        std::move(touchScreen),
        std::move(texturesRepository),
        std::move(physicsEngine)
    ),
    m_sceneManager(std::move(sceneManager))
{}

void RenderingEngineDevScene::update(float dt) {
    m_freeFlyCameraController->update();
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

    if (m_cameraButtonClickDetector->isClickDetected()) {
        switchCamera(!m_shouldUsePlayerCamera);
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

    auto movementJoystick = std::make_shared<SimpleJoystick>(
            findComponent<GestureConsumerComponent>("leftControllerArea"),
            m_unitsConverter,
            m_displayInfo,
            DpValue { 150 },
            DpValue { 150 }
    );
    m_freeFlyCameraController = std::make_shared<FreeFlyCameraController>(
            m_displayInfo,
            findComponent<TransformationComponent>("sceneCamera"),
            movementJoystick,
            m_gameObjectsMap.at("rightControllerArea")->findComponent<ScrollDetectorComponent>()
    );

    m_cameraButtonClickDetector = findComponent<ClickDetectorComponent>("cameraButton");

    m_player = m_gameObjectsMap.at("player");
    m_playerCamera = findComponent<PerspectiveCameraComponent>("playerCamera");

    m_freeFlyCamera = findComponent<PerspectiveCameraComponent>("sceneCamera");

    switchCamera(m_shouldUsePlayerCamera);
}

void RenderingEngineDevScene::switchCamera(bool shouldUsePlayerCamera) {
    m_shouldUsePlayerCamera = shouldUsePlayerCamera;

    if (m_shouldUsePlayerCamera) {
        m_playerCamera->setEnabled(true);
        m_freeFlyCamera->setEnabled(false);
    } else {
        m_playerCamera->setEnabled(false);
        m_freeFlyCamera->setEnabled(true);
    }
}
