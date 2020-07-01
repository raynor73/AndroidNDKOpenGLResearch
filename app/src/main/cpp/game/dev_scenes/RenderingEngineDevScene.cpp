//
// Created by Igor Lapin on 28/06/2020.
//

#include <exception>
#include <nlohmann/json.hpp>
#include <main/L.h>
#include "RenderingEngineDevScene.h"

void RenderingEngineDevScene::update(float dt) {

}

std::string RenderingEngineDevScene::createStateRepresentation() {
    throw std::domain_error("Not implemented");
}

void RenderingEngineDevScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    auto sceneJson = nlohmann::json::parse(stateRepresentation);

    auto gameObjectsJsonArray = sceneJson["gameObjects"];
    if (gameObjectsJsonArray.is_array()) {
        for (auto& gameObjectJson : gameObjectsJsonArray) {
            L::d("!@£", gameObjectJson["name"]);
        }
    }
}
