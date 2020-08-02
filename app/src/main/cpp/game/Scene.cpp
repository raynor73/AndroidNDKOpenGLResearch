//
// Created by Igor Lapin on 06/06/2020.
//

#include <utility>
#include <sstream>
#include <exception>
#include <main/L.h>
#include <main/Constants.h>
#include <exception>
#include <iomanip>
#include <glm/vec3.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <engine_3d/TransformationComponent.h>
#include <engine_3d/MeshComponent.h>
#include <engine_3d/OrthoCameraComponent.h>
#include <engine_3d/AmbientLightComponent.h>
#include <engine_3d/Material.h>
#include <engine_3d/MaterialComponent.h>
#include <engine_3d/TextComponent.h>
#include <engine_3d/ViewBoundsComponent.h>
#include <engine_3d/LayoutComponent.h>
#include <game/touch_screen/GestureConsumerComponent.h>
#include <game/touch_screen/ClickDetectorComponent.h>
#include <engine_3d/TextButtonComponent.h>
#include "Scene.h"

Scene::Scene(
        std::shared_ptr<TimeProvider> timeProvider,
        std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<UnitsConverter> unitsConverter,
        std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
        std::shared_ptr<MeshRendererFactory> meshRendererFactory,
        std::shared_ptr<TextRendererFactory> textRendererFactory,
        std::shared_ptr<TouchScreen> touchScreen,
        std::shared_ptr<TexturesRepository> texturesRepository
) :
    m_rootGameObject(std::make_shared<GameObject>("root")),
    m_timeProvider(std::move(timeProvider)),
    m_displayInfo(std::move(displayInfo)),
    m_prevTimestamp(0.0f),
    m_hasPrevTimestamp(false),
    m_unitsConverter(unitsConverter),
    m_meshLoadingRepository(meshLoadingRepository),
    m_meshRendererFactory(meshRendererFactory),
    m_textRendererFactory(textRendererFactory),
    m_touchScreen(touchScreen),
    m_texturesRepository(std::move(texturesRepository)),
    m_gesturesDispatcher(std::make_shared<GesturesDispatcher>())
{
    m_gameObjectsMap[m_rootGameObject->name()] = m_rootGameObject;
}

void Scene::update() {
    m_gesturesDispatcher->prepare();
    for (auto& event : m_touchScreen->events()) {
        m_gesturesDispatcher->onTouchEvent(event);
    }

    m_rootGameObject->update();

    auto currentTimestamp = m_timeProvider->getTimestamp();

    float dt;
    if (m_hasPrevTimestamp) {
        dt = (currentTimestamp - m_prevTimestamp) / TimeProvider::NANOS_IN_SECOND;
    } else {
        dt = 0.0f;
    }
    m_prevTimestamp = currentTimestamp;
    m_hasPrevTimestamp = true;

    update(dt);
}

void Scene::addGameObject(const std::string &parentName, std::shared_ptr<GameObject> gameObject) {
    if (m_gameObjectsMap.count(parentName) == 0) {
        std::stringstream ss;
        ss << "Parent game object " << parentName << " not found";
        throw std::domain_error(ss.str());
    }

    if (m_gameObjectsMap.count(gameObject->name()) > 0) {
        std::stringstream ss;
        ss << "Game object " << gameObject->name() << " already exists";
        throw std::domain_error(ss.str());
    }

    m_gameObjectsMap[parentName]->addChild(gameObject);
    m_gameObjectsMap[gameObject->name()] = gameObject;
}

void Scene::removeGameObject(const std::string &name) {
    if (m_gameObjectsMap.count(name) == 0) {
        std::stringstream ss;
        ss << "Game object " << name << " not found";
        throw std::domain_error(ss.str());
    }

    auto gameObject = m_gameObjectsMap[name];
    m_gameObjectsMap.erase(name);

    gameObject->parent()->removeChild(gameObject);
}

void Scene::restoreFromStateRepresentation(const std::string stateRepresentation) {
    std::unordered_map<std::string, Material> materialsMap;
    std::unordered_map<std::string, TextAppearance> textAppearancesMap;

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

    auto texturesJsonArray = sceneJson["textures"];
    if (texturesJsonArray.is_array()) {
        for (auto& textureJson : texturesJsonArray) {
            auto nameJson = textureJson["name"];
            if (!nameJson.is_string()) {
                continue;
            }
            auto pathJson = textureJson["path"];
            if (!pathJson.is_string()) {
                continue;
            }
            auto displayDensityFactorAwareJson = textureJson["displayDensityFactorAware"];
            bool displayDensityFactorAware =
                    displayDensityFactorAwareJson.is_boolean() &&
                    displayDensityFactorAwareJson.get<bool>();
            if (displayDensityFactorAware) {
                auto name = nameJson.get<std::string>();
                m_texturesRepository->createDisplayDensityFactorAwareTexture(
                        name, pathJson.get<std::string>()
                );
            } else {
                // TODO Implement later
            }
        }
    }

    auto materialsJsonArray = sceneJson["materials"];
    if (materialsJsonArray.is_array()) {
        for (auto& materialJson : materialsJsonArray) {
            auto nameJson = materialJson["name"];
            if (nameJson.is_null()) {
                continue;
            }
            auto diffuseColorJson = materialJson["diffuseColor"];
            if (diffuseColorJson.is_array()) {
                Material material { parseColor4f(materialJson["diffuseColor"]), "", true };
                materialsMap[nameJson.get<std::string>()] = material;
            } else {
                auto textureNameJson = materialJson["textureName"];
                if (!textureNameJson.is_string()) {
                    continue;
                }
                Material material { glm::vec4(0), textureNameJson.get<std::string>(), false };
                materialsMap[nameJson.get<std::string>()] = material;
            }
        }
    }

    auto textAppearancesJsonArray = sceneJson["textAppearances"];
    if (textAppearancesJsonArray.is_array()) {
        for (auto& textAppearanceJson : textAppearancesJsonArray) {
            auto nameJson = textAppearanceJson["name"];
            if (!nameJson.is_string()) {
                continue;
            }
            auto textSize = parseNumber(textAppearanceJson["textSize"], DimensionType::UNDEFINED);
            auto fontPathJson = textAppearanceJson["fontPath"];
            if (!fontPathJson.is_string()) {
                continue;
            }
            TextAppearance textAppearance { uint32_t(textSize), fontPathJson.get<std::string>() };
            textAppearancesMap.insert({ nameJson.get<std::string>(), textAppearance });
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
                    auto ptr = gameObject.get();
                    auto component = parseComponent(componentJson, materialsMap, textAppearancesMap);
                    if (ptr != gameObject.get()) {
                        throw std::domain_error("WTF");
                    }
                    gameObject->addComponent(component);
                }
            }
        }
    }
}

std::shared_ptr<GameObjectComponent> Scene::parseComponent(
        const nlohmann::json& componentJson,
        const std::unordered_map<std::string, Material>& materialsMap,
        const std::unordered_map<std::string, TextAppearance>& textAppearancesMap
) {
    if (!componentJson.contains("type")) {
        throw std::domain_error("No type found while parsing component");
    }
    auto type = componentJson["type"].get<std::string>();
    if (type == "Mesh") {
        auto meshNameJson = componentJson["meshName"];
        if (!meshNameJson.is_string()) {
            throw std::domain_error("No mesh name provided");
        }
        auto meshName = meshNameJson.get<std::string>();
        return std::make_shared<MeshComponent>(
                m_meshStorage.getMesh(meshName),
                meshName
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
    } else if (type == "Text") {
        auto textAppearanceNameJson = componentJson["textAppearanceName"];
        if (!textAppearanceNameJson.is_string()) {
            throw std::domain_error("No Text Appearance Name provided for Text Component");
        }
        auto textJson = componentJson["text"];
        if (!textJson.is_string()) {
            throw std::domain_error("No Text provided for Text Component");
        }

        auto textAppearanceName = textAppearanceNameJson.get<std::string>();
        if (textAppearancesMap.count(textAppearanceName) == 0) {
            std::stringstream ss;
            ss << "Can't find Text Appearance " << textAppearanceName;
            throw std::domain_error(ss.str());
        }
        return std::make_shared<TextComponent>(
                textJson.get<std::string>(),
                parseColor4f(componentJson["textColor"]),
                textAppearancesMap.at(textAppearanceName)
        );
    } else if (type == "TextRenderer") {
        return m_textRendererFactory->createTextRenderer(parseLayerNames(componentJson["layerNames"]));
    } else if (type == "ViewBounds") {
        auto left = parseNumber(componentJson["left"], DimensionType::WIDTH);
        auto bottom = parseNumber(componentJson["bottom"], DimensionType::HEIGHT);

        if (componentJson.contains("right") && componentJson.contains("top")) {
            auto right = parseNumber(componentJson["right"], DimensionType::WIDTH);
            auto top = parseNumber(componentJson["top"], DimensionType::HEIGHT);
            return std::make_shared<ViewBoundsComponent>(left, top, right, bottom);
        } else if (componentJson.contains("width") && componentJson.contains("height")) {
            auto width = parseNumber(componentJson["width"], DimensionType::WIDTH);
            auto height = parseNumber(componentJson["height"], DimensionType::HEIGHT);
            return std::make_shared<ViewBoundsComponent>(left, bottom + height, left + width, bottom);
        } else {
            throw std::domain_error("Insufficient params set for view bounds");
        }
    } else if (type == "Layout") {
        auto padding = parseVec4(componentJson["padding"]);

        auto verticalLayoutJson = componentJson["verticalLayout"];
        if (!verticalLayoutJson.is_string()) {
            throw std::domain_error("Vertical layout parameter not found");
        }
        auto horizontalLayoutJson = componentJson["horizontalLayout"];
        if (!horizontalLayoutJson.is_string()) {
            throw std::domain_error("Horizontal layout parameter not found");
        }
        auto originVerticalLayoutJson = componentJson["originVerticalLayout"];
        if (!originVerticalLayoutJson.is_string()) {
            throw std::domain_error("Origin vertical layout parameter not found");
        }
        auto originHorizontalLayoutJson = componentJson["originHorizontalLayout"];
        if (!originHorizontalLayoutJson.is_string()) {
            throw std::domain_error("Origin horizontal layout parameter not found");
        }

        auto referenceViewBoundsNameJson = componentJson["referenceViewBoundsName"];
        if (!referenceViewBoundsNameJson.is_string()) {
            throw std::domain_error("No reference view bounds name found");
        }
        auto referenceGameObjectName = referenceViewBoundsNameJson.get<std::string>();
        auto referenceGameObject = m_gameObjectsMap.at(referenceGameObjectName);
        if (referenceGameObject == nullptr) {
            std::stringstream ss;
            ss << "Reference game object " << referenceGameObjectName << " not found";
            throw std::domain_error(ss.str());
        }
        auto referenceViewBounds = std::static_pointer_cast<ViewBoundsComponent>(
                referenceGameObject->findComponent(ViewBoundsComponent::TYPE_NAME)
        );
        if (referenceViewBounds == nullptr) {
            std::stringstream ss;
            ss << "Reference view bounds " << referenceGameObjectName << " not found";
            throw std::domain_error(ss.str());
        }

        auto verticalLayoutString = verticalLayoutJson.get<std::string>();
        auto horizontalLayoutString = horizontalLayoutJson.get<std::string>();
        auto originVerticalLayoutString = originVerticalLayoutJson.get<std::string>();
        auto originHorizontalLayoutString = originHorizontalLayoutJson.get<std::string>();

        VerticalLayoutType verticalLayoutType;
        if (verticalLayoutString == "TOP_INSIDE") {
            verticalLayoutType = VerticalLayoutType::TOP_INSIDE;
        } else if (verticalLayoutString == "TOP_OUTSIDE") {
            verticalLayoutType = VerticalLayoutType::TOP_OUTSIDE;
        } else if (verticalLayoutString == "CENTER") {
            verticalLayoutType = VerticalLayoutType::CENTER;
        } else if (verticalLayoutString == "BOTTOM_INSIDE") {
            verticalLayoutType = VerticalLayoutType::BOTTOM_INSIDE;
        } else if (verticalLayoutString == "BOTTOM_OUTSIDE") {
            verticalLayoutType = VerticalLayoutType::BOTTOM_OUTSIDE;
        } else {
            std::stringstream ss;
            ss << "Unknown vertical layout " << verticalLayoutString;
            throw std::domain_error(ss.str());
        }

        HorizontalLayoutType horizontalLayoutType;
        if (horizontalLayoutString == "LEFT_INSIDE") {
            horizontalLayoutType = HorizontalLayoutType::LEFT_INSIDE;
        } else if (horizontalLayoutString == "LEFT_OUTSIDE") {
            horizontalLayoutType = HorizontalLayoutType::LEFT_OUTSIDE;
        } else if (horizontalLayoutString == "CENTER") {
            horizontalLayoutType = HorizontalLayoutType::CENTER;
        } else if (horizontalLayoutString == "RIGHT_INSIDE") {
            horizontalLayoutType = HorizontalLayoutType::RIGHT_INSIDE;
        } else if (horizontalLayoutString == "RIGHT_OUTSIDE") {
            horizontalLayoutType = HorizontalLayoutType::RIGHT_OUTSIDE;
        } else {
            std::stringstream ss;
            ss << "Unknown horizontal layout " << verticalLayoutString;
            throw std::domain_error(ss.str());
        }

        OriginVerticalLayoutType originVerticalLayoutType;
        if (originVerticalLayoutString == "TOP") {
            originVerticalLayoutType = OriginVerticalLayoutType::TOP;
        } else if (originVerticalLayoutString == "CENTER") {
            originVerticalLayoutType = OriginVerticalLayoutType::CENTER;
        } else if (originVerticalLayoutString == "BOTTOM") {
            originVerticalLayoutType = OriginVerticalLayoutType::BOTTOM;
        } else {
            std::stringstream ss;
            ss << "Unknown origin vertical layout " << verticalLayoutString;
            throw std::domain_error(ss.str());
        }

        OriginHorizontalLayoutType originHorizontalLayoutType;
        if (originHorizontalLayoutString == "LEFT") {
            originHorizontalLayoutType = OriginHorizontalLayoutType::LEFT;
        } else if (originHorizontalLayoutString == "CENTER") {
            originHorizontalLayoutType = OriginHorizontalLayoutType::CENTER;
        } else if (originHorizontalLayoutString == "RIGHT") {
            originHorizontalLayoutType = OriginHorizontalLayoutType::RIGHT;
        } else {
            std::stringstream ss;
            ss << "Unknown origin horizontal layout " << verticalLayoutString;
            throw std::domain_error(ss.str());
        }

        return std::make_shared<LayoutComponent>(LayoutParams {
                int(padding.x),
                int(padding.y),
                int(padding.z),
                int(padding.w),
                verticalLayoutType,
                horizontalLayoutType,
                originVerticalLayoutType,
                originHorizontalLayoutType,
                referenceViewBounds
        });
    } else if (type == "GestureConsumer") {
        auto priorityJson = componentJson["priority"];
        if (!priorityJson.is_number()) {
            throw std::domain_error("Can't find priority for gesture consumer");
        }
        auto gestureConsumer = std::make_shared<GestureConsumerComponent>(
                m_gesturesDispatcher,
                priorityJson.get<int>()
        );
        m_gesturesDispatcher->addGestureConsumer(gestureConsumer);
        return gestureConsumer;
    } else if (type == "ClickDetector") {
        return std::make_shared<ClickDetectorComponent>();
    } else if (type == "TextButton") {
        auto textColor = parseColor4f(componentJson["textColor"]);
        auto pressedTextColor = parseColor4f(componentJson["pressedTextColor"]);
        return std::make_shared<TextButtonComponent>(textColor, pressedTextColor);
    } else {
        std::stringstream ss;
        ss << "Unknown component type " << type;
        throw std::domain_error(ss.str());
    }
}

float Scene::parseFloatNumber(const nlohmann::json& jsonValue) {
    if (!jsonValue.is_number()) {
        throw std::domain_error("Float number has invalid format or missing");
    }
    return jsonValue.get<float>();
}

glm::vec4 Scene::parseVec4(const nlohmann::json& jsonValue) {
    if (!jsonValue.is_array() || jsonValue.size() != 4) {
        throw std::domain_error("Invalid or missing vec4 component");
    }

    return glm::vec4(
            parseNumber(jsonValue[0], DimensionType::WIDTH),
            parseNumber(jsonValue[1], DimensionType::HEIGHT),
            parseNumber(jsonValue[2], DimensionType::WIDTH),
            parseNumber(jsonValue[3], DimensionType::HEIGHT)
    );
}

glm::vec3 Scene::parseColor3f(const nlohmann::json &colorJson) {
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

glm::vec4 Scene::parseColor4f(const nlohmann::json &colorJson) {
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

std::vector<std::string> Scene::parseLayerNames(const nlohmann::json& layerNamesJsonArray) {
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

float Scene::parseNumber(const nlohmann::json &jsonValue, DimensionType dimensionType) {
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
