//
// Created by Igor Lapin on 29/06/2020.
//

#ifndef GAME_SCENELOADER_H
#define GAME_SCENELOADER_H


#include <nlohmann/json.hpp>

class SceneLoader {

public:
    SceneLoader() = default;
    SceneLoader(const SceneLoader&) = delete;
    SceneLoader(SceneLoader&&) = delete;



    SceneLoader& operator=(const SceneLoader&) = delete;
    SceneLoader& operator=(SceneLoader&&) = delete;
};


#endif //GAME_SCENELOADER_H
