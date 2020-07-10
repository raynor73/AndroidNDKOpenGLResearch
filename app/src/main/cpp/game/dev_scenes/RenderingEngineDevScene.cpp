//
// Created by Igor Lapin on 28/06/2020.
//

#include <exception>
#include <nlohmann/json.hpp>
#include <main/L.h>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <main/Constants.h>
#include <engine_3d/TransformationComponent.h>
#include <engine_3d/MeshComponent.h>
#include <engine_3d/OrthoCameraComponent.h>
#include <engine_3d/AmbientLightComponent.h>
#include <engine_3d/Material.h>
#include <engine_3d/MaterialComponent.h>
#include "RenderingEngineDevScene.h"

void RenderingEngineDevScene::update(float) {}

std::string RenderingEngineDevScene::createStateRepresentation() {
    throw std::domain_error("Not implemented");
}

void RenderingEngineDevScene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    std::unordered_map<std::string, Material> materialsMap;

    nlohmann::json sceneJson;
    try {
        sceneJson = nlohmann::json::parse(stateRepresentation);
    } catch (nlohmann::json::parse_error& e) {
        L::e(App::Constants::LOG_TAG, "Error restoring scene", e);
    }

    auto meshesJsonArray = sceneJson["meshes"];
    if (meshesJsonArray.is_array()) {
        for (auto& meshJson : meshesJsonArray) {
            std::string name;
            std::string path;

            if (!meshJson["name"].is_null()) {
                name = meshJson["name"].get<std::string>();
            } else {
                continue;
            }

            if (!meshJson["path"].is_null()) {
                path = meshJson["path"].get<std::string>();
            } else {
                continue;
            }

            m_meshStorage.putMesh(name, m_meshLoadingRepository->loadMesh(path));
        }
    }

    auto materialsJsonArray = sceneJson["materials"];
    if (materialsJsonArray.is_array()) {
        for (auto& materialJson : materialsJsonArray) {
            auto nameJson = materialJson["name"];
            if (nameJson.is_null()) {
                continue;
            }
            Material material { parseColor4f(materialJson["diffuseColor"]) };
            materialsMap[nameJson.get<std::string>()] = material;
        }
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

            auto rotationMatrix = glm::identity<glm::mat4>();
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
            auto rotationQuaternion = glm::quat_cast(rotationMatrix);

            std::shared_ptr<GameObject> gameObject;
            if (name != "root") {
                gameObject = std::make_shared<GameObject>(name);
                addGameObject(parentName, gameObject);
            } else {
                gameObject = m_rootGameObject;
            }
            auto transform = std::make_shared<TransformationComponent>(
                    position,
                    rotationQuaternion,
                    scale
            );
            gameObject->addComponent(transform);

            auto componentsJsonArray = gameObjectJson["components"];
            if (componentsJsonArray.is_array()) {
                for (auto& componentJson : componentsJsonArray) {
                    gameObject->addComponent(parseComponent(componentJson, materialsMap));
                }
            }
        }
    }
}

std::shared_ptr<GameObjectComponent> RenderingEngineDevScene::parseComponent(
        const nlohmann::json& componentJson,
        const std::unordered_map<std::string, Material>& materialsMap
) {
    if (!componentJson.contains("type")) {
        throw std::domain_error("No type found while parsing component");
    }
    auto type = componentJson["type"].get<std::string>();
    if (type == "Mesh") {
        if (componentJson["meshName"].is_null() || !componentJson["meshName"].is_string()) {
            throw std::domain_error("No mesh name provided");
        }
        return std::make_shared<MeshComponent>(
                m_meshStorage.getMesh(componentJson["meshName"].get<std::string>())
        );
    } else if (type == "Material") {
        if (!componentJson["materialName"].is_string()) {
            throw std::domain_error("No material name provided");
        }
        auto materialName = componentJson["materialName"].get<std::string>();
        if (materialsMap.count(materialName) == 0) {
            std::stringstream ss;
            ss << "Material " << materialName << " not found";
            throw std::domain_error(ss.str());
        }
        return std::make_shared<MaterialComponent>(materialsMap.at(materialName));
    } else if (type == "MeshRenderer") {
        return m_meshRendererFactory->createMeshRenderer(parseLayerNames(componentJson["layerNames"]));
    } else if (type == "OrthoCamera") {
        return std::make_shared<OrthoCameraComponent>(
                parseColor4f(componentJson["clearColor"]),
                parseLayerNames(componentJson["layerNames"]),
                parseNumber(componentJson["left"], DimensionType::WIDTH),
                parseNumber(componentJson["top"], DimensionType::HEIGHT),
                parseNumber(componentJson["right"], DimensionType::WIDTH),
                parseNumber(componentJson["bottom"], DimensionType::HEIGHT),
                parseFloatNumber(componentJson["zNear"]),
                parseFloatNumber(componentJson["zFar"])
        );
    } else if (type == "AmbientLight") {
        return std::make_shared<AmbientLightComponent>(
                parseColor3f(componentJson["color"]),
                parseLayerNames(componentJson["layerNames"])
        );
    } else {
        std::stringstream ss;
        ss << "Unknown component type " << type;
        throw std::domain_error(ss.str());
    }
}

float RenderingEngineDevScene::parseFloatNumber(const nlohmann::json& jsonValue) {
    if (!jsonValue.is_number()) {
        throw std::domain_error("Float number has invalid format or missing");
    }
    return jsonValue.get<float>();
}

glm::vec3 RenderingEngineDevScene::parseColor3f(const nlohmann::json &colorJson) {
    if (!colorJson.is_array() || colorJson.size() != 3) {
        throw std::domain_error("Invalid or missing 3 components color");
    }

    if (!colorJson[0].is_number()) {
        throw std::domain_error("Invalid red component of 3 components color");
    }
    auto red = colorJson[0].get<float>();

    if (!colorJson[1].is_number()) {
        throw std::domain_error("Invalid green component of 3 components color");
    }
    auto green = colorJson[1].get<float>();

    if (!colorJson[2].is_number()) {
        throw std::domain_error("Invalid blue component of 3 components color");
    }
    auto blue = colorJson[2].get<float>();

    return glm::vec3 { red, green, blue };
}

glm::vec4 RenderingEngineDevScene::parseColor4f(const nlohmann::json &colorJson) {
    if (!colorJson.is_array() || colorJson.size() != 4) {
        throw std::domain_error("Invalid or missing 4 components color");
    }

    if (!colorJson[0].is_number()) {
        throw std::domain_error("Invalid red component of 4 components color");
    }
    auto red = colorJson[0].get<float>();

    if (!colorJson[1].is_number()) {
        throw std::domain_error("Invalid green component of 4 components color");
    }
    auto green = colorJson[1].get<float>();

    if (!colorJson[2].is_number()) {
        throw std::domain_error("Invalid blue component of 4 components color");
    }
    auto blue = colorJson[2].get<float>();

    if (!colorJson[3].is_number()) {
        throw std::domain_error("Invalid alpha component of 4 components color");
    }
    auto alpha = colorJson[3].get<float>();

    return glm::vec4 { red, green, blue, alpha };
}

std::vector<std::string> RenderingEngineDevScene::parseLayerNames(const nlohmann::json& layerNamesJsonArray) {
    if (layerNamesJsonArray.is_null() || !layerNamesJsonArray.is_array()) {
        throw std::domain_error("Bad layer names JSON");
    }
    std::vector<std::string> layerNames;
    for (auto &layerNameJson : layerNamesJsonArray) {
        if (!layerNameJson.is_string()) {
            throw std::domain_error("String expected as layer name");
        }
        layerNames.push_back(layerNameJson.get<std::string>());
    }
    return layerNames;
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
