//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H


#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <engine_3d/GameObject.h>
#include <engine_3d/Material.h>
#include <engine_3d/TextAppearance.h>
#include <game/UnitsConverter.h>
#include <game/MeshLoadingRepository.h>
#include <game/MeshStorage.h>
#include <game/MeshRendererFactory.h>
#include <game/TextRendererFactory.h>
#include <game/touch_screen/TouchScreen.h>
#include <game/touch_screen/GesturesDispatcher.h>
#include "TimeProvider.h"
#include "DisplayInfo.h"

class Scene {

    float m_prevTimestamp;
    bool m_hasPrevTimestamp;

public:
    explicit Scene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory,
            std::shared_ptr<TextRendererFactory> textRendererFactory,
            std::shared_ptr<TouchScreen> touchScreen
    );
    virtual ~Scene() = default;

    std::shared_ptr<GameObject> rootGameObject() { return m_rootGameObject; }

    virtual std::string createStateRepresentation() { throw std::domain_error("Not implemented"); };
    virtual void restoreFromStateRepresentation(const std::string stateRepresentation);

    void update();

protected:
    std::shared_ptr<UnitsConverter> m_unitsConverter;
    std::shared_ptr<MeshLoadingRepository> m_meshLoadingRepository;
    std::shared_ptr<MeshRendererFactory> m_meshRendererFactory;
    std::shared_ptr<TextRendererFactory> m_textRendererFactory;
    std::shared_ptr<TouchScreen> m_touchScreen;

    MeshStorage m_meshStorage;
    GesturesDispatcher m_gesturesDispatcher;

    std::shared_ptr<GameObject> m_rootGameObject;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_gameObjectsMap;
    std::shared_ptr<TimeProvider> m_timeProvider;
    std::shared_ptr<DisplayInfo> m_displayInfo;

    virtual void update(float dt) = 0;

    void addGameObject(const std::string& parentName, std::shared_ptr<GameObject> gameObject);
    void removeGameObject(const std::string& name);

private:
    enum class DimensionType {
        UNDEFINED, WIDTH, HEIGHT
    };

    float parseNumber(const nlohmann::json& jsonValue, DimensionType dimensionType);
    static std::vector<std::string> parseLayerNames(const nlohmann::json& layerNamesJsonArray);
    std::shared_ptr<GameObjectComponent> parseComponent(
            const nlohmann::json& componentJson,
            const std::unordered_map<std::string, Material>& materialsMap,
            const std::unordered_map<std::string, TextAppearance>& textAppearancesMap
    );
    static glm::vec3 parseColor3f(const nlohmann::json& colorJson);
    static glm::vec4 parseColor4f(const nlohmann::json& colorJson);
    glm::vec4 parseVec4(const nlohmann::json& jsonValue);
    static float parseFloatNumber(const nlohmann::json& jsonValue);
};


#endif //GAME_SCENE_H
