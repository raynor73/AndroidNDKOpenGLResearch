//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINEDEVSCENE_H
#define GAME_RENDERINGENGINEDEVSCENE_H


#include <game/Scene.h>
#include <memory>
#include <game/FpsCalculator.h>
#include <game/touch_screen/ClickDetectorComponent.h>
#include <game/SceneManager.h>
#include <engine_3d/TransformationComponent.h>
#include <game/touch_screen/ScrollDetectorComponent.h>
#include <engine_3d/Utils.h>
#include <game/SimpleJoystick.h>
#include <engine_3d/PhysicsEngine.h>
#include <game/FreeFlyCameraController.h>
#include <engine_3d/PerspectiveCameraComponent.h>
#include <game/PlayerController.h>
#include "SceneCloser.h"

class RenderingEngineDevScene : public Scene {

    std::shared_ptr<SceneManager> m_sceneManager;

    FpsCalculator m_fpsCalculator;
    std::shared_ptr<SceneCloser> m_sceneCloser;

    std::shared_ptr<TransformationComponent> m_boxTransform;
    std::shared_ptr<TransformationComponent> m_box2Transform;

    std::shared_ptr<GameObject> m_ballPrefab;

    std::shared_ptr<SimpleJoystick> m_movementJoystick;

    std::shared_ptr<PerspectiveCameraComponent> m_freeFlyCamera;
    std::shared_ptr<FreeFlyCameraController> m_freeFlyCameraController;

    std::shared_ptr<PerspectiveCameraComponent> m_playerCamera;
    std::shared_ptr<PlayerController> m_playerController;

    std::shared_ptr<ClickDetectorComponent> m_cameraButtonClickDetector;
    bool m_shouldUsePlayerCamera = false;

    float m_boxAngleX = 0;
    float m_boxAngleY = 0;
    float m_boxAngleZ = 0;

    float m_box2AngleX = 0;
    float m_box2AngleY = 0;
    float m_box2AngleZ = 0;

public:
    RenderingEngineDevScene(
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
    );

    virtual void update(float dt) override;

    virtual void restoreFromStateRepresentation(const std::string stateRepresentation) override;

private:
    template<typename T>
    std::shared_ptr<T> findComponent(const std::string& gameObjectName) {
        auto gameObject = m_gameObjectsMap.at(gameObjectName);
        Engine3D::Utils::throwErrorIfNull(gameObject, "Game object not found");
        auto component = std::static_pointer_cast<T>(gameObject->findComponent(T::TYPE_NAME));
        Engine3D::Utils::throwErrorIfNull(m_boxTransform, "Game object has no required component");
        return component;
    }

    void switchCamera(bool shouldUsePlayerCamera);
};


#endif //GAME_RENDERINGENGINEDEVSCENE_H
