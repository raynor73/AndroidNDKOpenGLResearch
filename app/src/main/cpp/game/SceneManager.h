//
// Created by Igor Lapin on 27/07/2020.
//

#ifndef GAME_SCENE_MANAGER_H
#define GAME_SCENE_MANAGER_H


enum class SceneType {
    SCENES_SELECTION_SCENE, RENDERING_ENGINE_DEV_SCENE, SCREEN_BLINKING_SCENE, MULTITOUCH_TEST_SCENE
};

class SceneManager {

public:
    virtual ~SceneManager() = default;

    virtual void requestSceneLoadAndStart(SceneType type) = 0;
};


#endif //GAME_SCENE_MANAGER_H
