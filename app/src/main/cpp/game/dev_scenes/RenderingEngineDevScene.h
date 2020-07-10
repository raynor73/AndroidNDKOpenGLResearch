//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINEDEVSCENE_H
#define GAME_RENDERINGENGINEDEVSCENE_H


#include <game/Scene.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <game/UnitsConverter.h>
#include <game/MeshLoadingRepository.h>
#include <game/MeshStorage.h>
#include <game/MeshRendererFactory.h>
#include <engine_3d/Material.h>

class RenderingEngineDevScene : public Scene {

    std::shared_ptr<UnitsConverter> m_unitsConverter;
    std::shared_ptr<MeshLoadingRepository> m_meshLoadingRepository;
    std::shared_ptr<MeshRendererFactory> m_meshRendererFactory;
    MeshStorage m_meshStorage;

public:
    RenderingEngineDevScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository,
            std::shared_ptr<MeshRendererFactory> meshRendererFactory
    ) : Scene(timeProvider, displayInfo),
        m_unitsConverter(unitsConverter),
        m_meshLoadingRepository(meshLoadingRepository),
        m_meshRendererFactory(meshRendererFactory) {}

    virtual std::string createStateRepresentation() override;
    virtual void restoreFromStateRepresentation(const std::string stateRepresentation) override;

    virtual void update(float dt) override;

private:
    enum class DimensionType {
        UNDEFINED, WIDTH, HEIGHT
    };

    float parseNumber(const nlohmann::json& jsonValue, DimensionType dimensionType);
    static std::vector<std::string> parseLayerNames(const nlohmann::json& layerNamesJsonArray);
    std::shared_ptr<GameObjectComponent> parseComponent(
            const nlohmann::json& componentJson,
            const std::unordered_map<std::string, Material>& materialsMap
    );
    static glm::vec3 parseColor3f(const nlohmann::json& colorJson);
    static glm::vec4 parseColor4f(const nlohmann::json& colorJson);
    static float parseFloatNumber(const nlohmann::json& jsonValue);
};


#endif //GAME_RENDERINGENGINEDEVSCENE_H
