//
// Created by Igor Lapin on 27/07/2020.
//

#ifndef GAME_SCENE_MANAGER_H
#define GAME_SCENE_MANAGER_H


#include <unordered_map>

enum class SceneType {
    SCENES_SELECTION_SCENE, LOADING_SCENE, RENDERING_ENGINE_DEV_SCENE, SCREEN_BLINKING_SCENE, MULTITOUCH_TEST_SCENE
};

typedef std::variant<std::string, char, int, long, float, std::vector<uint8_t>> RequestedSceneArgValue;

class SceneManager {

public:
    virtual ~SceneManager() = default;

    virtual void requestSceneLoadAndStart(
            SceneType type,
            std::unordered_map<std::string, RequestedSceneArgValue> args
    ) = 0;

    virtual void requestSceneLoadAndStart(SceneType type) = 0;
};


#endif //GAME_SCENE_MANAGER_H
