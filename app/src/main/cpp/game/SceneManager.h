//
// Created by Igor Lapin on 27/07/2020.
//

#ifndef GAME_SCENE_MANAGER_H
#define GAME_SCENE_MANAGER_H


enum class SceneType {
    RENDERING_ENGINE_DEV_SCENE, SCREEN_BLINKING_SCENE
};

class SceneManager {

public:
    virtual ~SceneManager() = default;

    virtual void loadAndStartScene(SceneType type) = 0;
};


#endif //GAME_SCENE_MANAGER_H
