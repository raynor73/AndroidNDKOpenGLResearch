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
#include <game/TexturesRepository.h>
#include <main/L.h>
#include <engine_3d/PhysicsEngine.h>
#include "TimeProvider.h"
#include "DisplayInfo.h"
#include "SkeletalAnimationStorage.h"
#include <game/SkeletalAnimationLoadingRepository.h>
#include <engine_3d/SoundLoadingRepository.h>
#include <engine_3d/SoundStorage.h>

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
            std::shared_ptr<TouchScreen> touchScreen,
            std::shared_ptr<TexturesRepository> texturesRepository,
            std::shared_ptr<PhysicsEngine> physicsEngine,
            std::shared_ptr<SkeletalAnimationLoadingRepository> skeletalAnimationLoadingRepository,
            std::shared_ptr<SoundLoadingRepository> soundLoadingRepository
    );
    virtual ~Scene() = default;

    std::shared_ptr<GameObject> rootGameObject() { return m_rootGameObject; }

    virtual void buildHierarchyFromRepresentation(const std::string& hierarchyRepresentation);

    void update();

protected:
    std::shared_ptr<UnitsConverter> m_unitsConverter;
    std::shared_ptr<MeshLoadingRepository> m_meshLoadingRepository;
    std::shared_ptr<MeshRendererFactory> m_meshRendererFactory;
    std::shared_ptr<TextRendererFactory> m_textRendererFactory;
    std::shared_ptr<TouchScreen> m_touchScreen;
    std::shared_ptr<TexturesRepository> m_texturesRepository;
    std::shared_ptr<PhysicsEngine> m_physicsEngine;
    std::shared_ptr<SkeletalAnimationLoadingRepository> m_skeletalAnimationLoadingRepository;
    std::shared_ptr<SoundLoadingRepository> m_soundLoadingRepository;

    MeshStorage m_meshStorage;
    SoundStorage m_soundStorage;
    SkeletalAnimationStorage m_skeletalAnimationStorage;
    std::shared_ptr<GesturesDispatcher> m_gesturesDispatcher;

    std::shared_ptr<GameObject> m_rootGameObject;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_gameObjectsMap;
    std::shared_ptr<TimeProvider> m_timeProvider;
    std::shared_ptr<DisplayInfo> m_displayInfo;

    virtual void update(float dt) = 0;

    void addGameObject(const std::string& parentName, std::shared_ptr<GameObject> gameObject);
    void removeGameObject(const std::string& name);

private:
    static ComplexValue parseComplexValue(
            const nlohmann::json& jsonValue,
            std::optional<DimensionType> optionalDimensionType = std::optional<DimensionType>()
    );
    static std::vector<std::string> parseLayerNames(const nlohmann::json& layerNamesJsonArray);
    std::shared_ptr<GameObjectComponent> parseComponent(
            const std::shared_ptr<GameObject>& gameObject,
            const nlohmann::json& componentJson,
            const std::unordered_map<std::string, Material>& materialsMap,
            const std::unordered_map<std::string, TextAppearance>& textAppearancesMap
    );
    static glm::vec3 parseColor3f(const nlohmann::json& colorJson);
    static glm::vec4 parseColor4f(const nlohmann::json& colorJson);
    static float parseFloatNumber(const nlohmann::json& jsonValue);
    static Mesh createTransformedMesh(
            const Mesh& mesh,
            const glm::vec3& position,
            const glm::quat& rotation,
            const glm::vec3& scale
    );
};


#endif //GAME_SCENE_H
