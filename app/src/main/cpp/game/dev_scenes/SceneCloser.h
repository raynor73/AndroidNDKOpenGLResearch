//
// Created by Igor Lapin on 03/08/2020.
//

#ifndef GAME_SCENE_CLOSER_H
#define GAME_SCENE_CLOSER_H


#include <memory>
#include <utility>
#include <game/touch_screen/ClickDetectorComponent.h>
#include <game/SceneManager.h>
#include <engine_3d/GameObject.h>

class   SceneCloser {

    std::shared_ptr<SceneManager> m_sceneManager;
    std::shared_ptr<ClickDetectorComponent> m_closeButtonClickDetector;

    SceneType m_parentSceneType;

public:
    SceneCloser(
            std::shared_ptr<SceneManager> sceneManager,
            const std::shared_ptr<GameObject>& closeButton,
            SceneType parentSceneType
    );

    void update();
};


#endif //GAME_SCENE_CLOSER_H
