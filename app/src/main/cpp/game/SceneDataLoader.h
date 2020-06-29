//
// Created by Igor Lapin on 29/06/2020.
//

#ifndef GAME_SCENELOADER_H
#define GAME_SCENELOADER_H


#include <string>
#include <nlohmann/json.hpp>
#include "Scene.h"

class SceneDataLoader {

public:
    SceneDataLoader() = default;
    SceneDataLoader(const SceneDataLoader&) = delete;
    SceneDataLoader(SceneDataLoader&&) = delete;
    virtual ~SceneDataLoader() = default;

    virtual void loadSceneData(const std::string& path, Scene& scene) = 0;

    SceneDataLoader& operator=(const SceneDataLoader&) = delete;
    SceneDataLoader& operator=(SceneDataLoader&&) = delete;
};


#endif //GAME_SCENELOADER_H
