//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINEDEVSCENE_H
#define GAME_RENDERINGENGINEDEVSCENE_H


#include <engine_3d/Scene.h>
#include <memory>
#include <game/FpsCalculator.h>
#include <engine_3d/touch_screen/ClickDetectorComponent.h>
#include <engine_3d/SceneManager.h>
#include <engine_3d/TransformationComponent.h>
#include <engine_3d/touch_screen/ScrollDetectorComponent.h>
#include <engine_3d/Utils.h>
#include <game/SimpleJoystick.h>
#include <engine_3d/PhysicsEngine.h>
#include <game/FreeFlyCameraController.h>
#include <engine_3d/PerspectiveCameraComponent.h>
#include <game/PlayerController.h>
#include <engine_3d/FsAbstraction.h>
#include <engine_3d/SoundListenerComponent.h>
#include <engine_3d/ValueAnimator.h>
#include "SceneCloser.h"

class RenderingEngineDevScene : public Scene {

    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<FsAbstraction> m_fsAbstraction;

    FpsCalculator m_fpsCalculator;
    std::shared_ptr<SceneCloser> m_sceneCloser;

    std::shared_ptr<TransformationComponent> m_boxTransform;
    std::shared_ptr<TransformationComponent> m_box2Transform;
    std::shared_ptr<TransformationComponent> m_box4Transform;

    std::shared_ptr<GameObject> m_ballPrefab;

    std::shared_ptr<SimpleJoystick> m_movementJoystick;

    std::shared_ptr<PerspectiveCameraComponent> m_freeFlyCamera;
    std::shared_ptr<SoundListenerComponent> m_freeFlySoundListener;
    std::shared_ptr<FreeFlyCameraController> m_freeFlyCameraController;

    std::shared_ptr<PerspectiveCameraComponent> m_playerCamera;
    std::shared_ptr<SoundListenerComponent> m_playerSoundListener;
    std::shared_ptr<PlayerController> m_playerController;

    std::shared_ptr<ClickDetectorComponent> m_cameraButtonClickDetector;
    bool m_shouldUsePlayerCamera = false;

    std::shared_ptr<ClickDetectorComponent> m_saveButtonClickDetector;

    std::shared_ptr<GameObject> m_deleteButton;
    std::shared_ptr<ClickDetectorComponent> m_deleteButtonClickDetector;

    ValueAnimator m_box4Animator;

    float m_boxAngleX = 0;
    float m_boxAngleY = 0;
    float m_boxAngleZ = 0;

    float m_box2AngleX = 0;
    float m_box2AngleY = 0;
    float m_box2AngleZ = 0;

public:
    RenderingEngineDevScene(
            std::shared_ptr<Time> time,
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
            std::shared_ptr<FsAbstraction> fsAbstraction,
            std::shared_ptr<SoundStorage> soundStorage,
            std::shared_ptr<SoundScene> soundScene,
            std::shared_ptr<AppStateRepository> appStateRepository
    );

    virtual void update() override;

    virtual void buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation) override;

private:
    static const std::string DYNAMIC_STATE_FILE_PATH;

    template<typename T>
    std::shared_ptr<T> findComponent(const std::string& gameObjectName) {
        auto gameObject = m_gameObjectsMap.at(gameObjectName);
        Engine3D::Utils::throwErrorIfNull(gameObject, "Game object not found");
        auto component = std::static_pointer_cast<T>(gameObject->findComponent(T::TYPE_NAME));
        Engine3D::Utils::throwErrorIfNull(component, "Game object has no required component");
        return component;
    }

    void switchCamera(bool shouldUsePlayerCamera);

    void updateDeleteButtonVisibility();
};


#endif //GAME_RENDERINGENGINEDEVSCENE_H
