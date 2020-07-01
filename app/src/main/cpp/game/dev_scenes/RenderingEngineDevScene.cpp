//
// Created by Igor Lapin on 28/06/2020.
//

#include <exception>
#include <nlohmann/json.hpp>
#include <main/L.h>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <main/Constants.h>
#include <engine_3d/TransformationComponent.h>
#include "RenderingEngineDevScene.h"

void RenderingEngineDevScene::update(float dt) {}

std::string RenderingEngineDevScene::createStateRepresentation() {
    throw std::domain_error("Not implemented");
}

void RenderingEngineDevScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    nlohmann::json sceneJson;
    try {
        sceneJson = nlohmann::json::parse(stateRepresentation);
    } catch (nlohmann::json::parse_error& e) {
        L::e(App::Constants::LOG_TAG, "Error restoring scene", e);
    }

    auto gameObjectsJsonArray = sceneJson["gameObjects"];
    if (gameObjectsJsonArray.is_array()) {
        for (auto& gameObjectJson : gameObjectsJsonArray) {
            std::string name;
            std::string parentName;
            glm::vec3 position;
            glm::quat rotation;
            glm::vec3 scale;

            if (gameObjectJson["name"].is_string()) {
                name = gameObjectJson["name"].get<std::string>();
            } else {
                continue;
            }

            if (gameObjectJson["parent"].is_string()) {
                parentName = gameObjectJson["parent"].get<std::string>();
            } else {
                continue;
            }

            if (gameObjectJson["position"].is_array() && gameObjectJson["position"].size() == 3) {
                try {
                    position.x = parseNumber(gameObjectJson["position"][0], DimensionType::WIDTH);
                    position.y = parseNumber(gameObjectJson["position"][1], DimensionType::HEIGHT);
                    position.z = parseNumber(gameObjectJson["position"][2], DimensionType::UNDEFINED);
                } catch (std::exception& e) {
                    L::e(App::Constants::LOG_TAG, "Error parsing position values", e);
                    continue;
                }
            } else {
                continue;
            }

            if (gameObjectJson["rotation"].is_array() && gameObjectJson["rotation"].size() == 3) {
                try {
                    rotation.x = parseNumber(gameObjectJson["rotation"][0], DimensionType::WIDTH);
                    rotation.y = parseNumber(gameObjectJson["rotation"][1], DimensionType::HEIGHT);
                    rotation.z = parseNumber(gameObjectJson["rotation"][2], DimensionType::UNDEFINED);
                } catch (std::exception& e) {
                    L::e(App::Constants::LOG_TAG, "Error parsing rotation values", e);
                    continue;
                }
            } else {
                continue;
            }

            if (gameObjectJson["scale"].is_array() && gameObjectJson["scale"].size() == 3) {
                try {
                    scale.x = parseNumber(gameObjectJson["scale"][0], DimensionType::WIDTH);
                    scale.y = parseNumber(gameObjectJson["scale"][1], DimensionType::HEIGHT);
                    scale.z = parseNumber(gameObjectJson["scale"][2], DimensionType::UNDEFINED);
                } catch (std::exception& e) {
                    L::e(App::Constants::LOG_TAG, "Error parsing scale values", e);
                    continue;
                }
            } else {
                continue;
            }

            if (name != "root") {
                auto gameObject = std::make_shared<GameObject>(name);
                auto transform = std::make_shared<TransformationComponent>(
                        position,
                        glm::orirotation,
                        scale
                );
                addGameObject(parentName, gameObject);
            }
        }
    }
}

float RenderingEngineDevScene::parseNumber(const nlohmann::json &jsonValue, DimensionType dimensionType) {
    if (jsonValue.is_number()) {
        return jsonValue.get<float>();
    } else if (jsonValue.is_string()) {
        auto stringValue = jsonValue.get<std::string>();
        if (stringValue[stringValue.size() - 1] == '%') {
            auto value = std::stof(stringValue.substr(0, stringValue.size() - 1));
            switch (dimensionType) {
                case DimensionType::UNDEFINED:
                    throw std::domain_error("Percent value given but dimension is undefined");

                case DimensionType::WIDTH:
                    return m_unitsConverter->widthPercentToPixels(value);

                case DimensionType::HEIGHT:
                    return m_unitsConverter->heightPercentToPixels(value);
            }
        } else if (stringValue.substr(stringValue.size() - 2, 2) == "dp") {
            return m_unitsConverter->dpToPixels(std::stof(stringValue.substr(0, stringValue.size() - 2)));
        } else {
            throw std::domain_error("Bad number format");
        }
    } else {
        throw std::domain_error("JSON value is not a number or string");
    }
}
