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
#include <engine_3d/skeletal_animation/SkeletalAnimationPlayerComponent.h>
#include <nlohmann/json.hpp>
#include <main/Constants.h>
#include "RenderingEngineDevScene.h"

using namespace Engine3D::Utils;

const std::string RenderingEngineDevScene::DYNAMIC_STATE_FILE_PATH = "rendering_engine_dev_scene_dynamic_state.json";

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
        std::shared_ptr<PhysicsEngine> physicsEngine,
        std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationRepository,
        std::shared_ptr<SoundLoadingRepository> soundLoadingRepository,
        std::shared_ptr<FsAbstraction> fsAbstraction
) : Scene(
        std::move(timeProvider),
        std::move(displayInfo),
        std::move(unitsConverter),
        std::move(meshLoadingRepository),
        std::move(meshRendererFactory),
        std::move(textRendererFactory),
        std::move(touchScreen),
        std::move(texturesRepository),
        std::move(physicsEngine),
        std::move(skeletalAnimationRepository),
        std::move(soundLoadingRepository)
    ),
    m_sceneManager(std::move(sceneManager)),
    m_fsAbstraction(std::move(fsAbstraction))
{}

void RenderingEngineDevScene::update(float dt) {
    m_movementJoystick->update();
    m_freeFlyCameraController->update();
    m_playerController->update();
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

    m_boxTransform->setRotation(
            eulerZXY(glm::radians(m_boxAngleY), glm::radians(m_boxAngleZ), glm::radians(m_boxAngleX))
    );

    m_box2Transform->setRotation(
            eulerZXY(glm::radians(m_box2AngleY), glm::radians(m_box2AngleZ), glm::radians(m_box2AngleX))
    );

    if (m_cameraButtonClickDetector->isClickDetected()) {
        switchCamera(!m_shouldUsePlayerCamera);
    }

    if (m_saveButtonClickDetector->isClickDetected()) {
        nlohmann::json dynamicStateJson;

        auto cameraTransform = m_freeFlyCamera->gameObject()->findComponent<TransformationComponent>();
        dynamicStateJson["cameraTransform"] = {
                {
                    "position", nlohmann::json::array({
                        cameraTransform->position().x,
                        cameraTransform->position().y,
                        cameraTransform->position().z
                    }),
                },
                { "cameraAngleX", m_freeFlyCameraController->cameraAngleX() },
                { "cameraAngleY", m_freeFlyCameraController->cameraAngleY() }
        };

        auto playerTransform = findComponent<TransformationComponent>("player");
        dynamicStateJson["playerTransform"] = {
                {
                        "position", nlohmann::json::array({
                            playerTransform->position().x,
                            playerTransform->position().y,
                            playerTransform->position().z
                        }),
                },
                { "playerAngle", m_playerController->playerAngle() }
        };

        m_fsAbstraction->writeTextFileContent(DYNAMIC_STATE_FILE_PATH, dynamicStateJson.dump(4));

        updateDeleteButtonVisibility();
    }

    if (m_deleteButtonClickDetector->isClickDetected()) {
        m_fsAbstraction->deleteFile(DYNAMIC_STATE_FILE_PATH);
        updateDeleteButtonVisibility();
    }
}

void RenderingEngineDevScene::buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation) {
    Scene::buildHierarchyFromRepresentation(hierarchyRepresentation);

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

    m_ballPrefab = m_gameObjectsMap.at("ballPrefab");
    removeGameObject("ballPrefab");

    m_movementJoystick = std::make_shared<SimpleJoystick>(
            findComponent<GestureConsumerComponent>("leftControllerArea"),
            m_unitsConverter,
            m_displayInfo,
            DpValue { 150 },
            DpValue { 150 }
    );
    m_freeFlyCameraController = std::make_shared<FreeFlyCameraController>(
            m_displayInfo,
            findComponent<TransformationComponent>("sceneCamera"),
            m_movementJoystick,
            findComponent<ScrollDetectorComponent>("rightControllerArea")
    );
    m_freeFlyCamera = findComponent<PerspectiveCameraComponent>("sceneCamera");

    m_cameraButtonClickDetector = findComponent<ClickDetectorComponent>("cameraButton");

    m_playerController = std::make_shared<PlayerController>(
            m_physicsEngine,
            m_displayInfo,
            m_movementJoystick,
            findComponent<ScrollDetectorComponent>("rightControllerArea"),
            findComponent<TransformationComponent>("playerRotor"),
            m_gameObjectsMap.at("playerRotor"),
            m_gameObjectsMap.at("femaleIdle"),
            m_gameObjectsMap.at("femaleRunning")
    );
    m_playerCamera = findComponent<PerspectiveCameraComponent>("playerCamera");

    switchCamera(m_shouldUsePlayerCamera);

    m_saveButtonClickDetector = findComponent<ClickDetectorComponent>("saveButton");

    m_deleteButton = m_gameObjectsMap.at("deleteButton");
    m_deleteButtonClickDetector = findComponent<ClickDetectorComponent>("deleteButton");

    if (m_fsAbstraction->isFileExists(DYNAMIC_STATE_FILE_PATH)) {
        try {
            auto dynamicStateJson = nlohmann::json::parse(m_fsAbstraction->readTextFileContent(DYNAMIC_STATE_FILE_PATH));

            auto cameraTransformJson = dynamicStateJson["cameraTransform"];
            auto playerTransformJson = dynamicStateJson["playerTransform"];

            auto cameraAngleX = cameraTransformJson["cameraAngleX"].get<float>();
            auto cameraAngleY = cameraTransformJson["cameraAngleY"].get<float>();

            auto cameraPositionJson = cameraTransformJson["position"];
            auto cameraPosition = glm::vec3(
                    cameraPositionJson[0].get<float>(),
                    cameraPositionJson[1].get<float>(),
                    cameraPositionJson[2].get<float>()
            );

            auto playerAngle = playerTransformJson["playerAngle"].get<float>();

            auto playerPositionJson = playerTransformJson["position"];
            auto playerPosition = glm::vec3(
                    playerPositionJson[0].get<float>(),
                    playerPositionJson[1].get<float>(),
                    playerPositionJson[2].get<float>()
            );

            auto cameraTransform = m_freeFlyCamera->gameObject()->findComponent<TransformationComponent>();
            cameraTransform->setPosition(cameraPosition);
            m_freeFlyCameraController->setCameraAngleX(cameraAngleX);
            m_freeFlyCameraController->setCameraAngleY(cameraAngleY);

            m_playerController->setPlayerAngle(playerAngle);
            m_physicsEngine->setPosition("player", playerPosition);
        } catch (...) {
            L::e(App::Constants::LOG_TAG, "Error restoring dynamic state");
        }
    }

    updateDeleteButtonVisibility();
}

void RenderingEngineDevScene::switchCamera(bool shouldUsePlayerCamera) {
    m_shouldUsePlayerCamera = shouldUsePlayerCamera;

    if (m_shouldUsePlayerCamera) {
        m_playerCamera->setEnabled(true);
        m_playerController->setEnabled(true);

        m_freeFlyCamera->setEnabled(false);
        m_freeFlyCameraController->setEnabled(false);
    } else {
        m_playerCamera->setEnabled(false);
        m_playerController->setEnabled(false);

        m_freeFlyCamera->setEnabled(true);
        m_freeFlyCameraController->setEnabled(true);
    }
}

void RenderingEngineDevScene::updateDeleteButtonVisibility() {
    m_deleteButton->setEnabled(m_fsAbstraction->isFileExists(DYNAMIC_STATE_FILE_PATH));
}
