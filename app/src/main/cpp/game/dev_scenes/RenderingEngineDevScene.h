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

class RenderingEngineDevScene : public Scene {

    std::shared_ptr<UnitsConverter> m_unitsConverter;
    std::shared_ptr<MeshLoadingRepository> m_meshLoadingRepository;

public:
    RenderingEngineDevScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<MeshLoadingRepository> meshLoadingRepository
    ) : Scene(timeProvider, displayInfo),
        m_unitsConverter(unitsConverter),
        m_meshLoadingRepository(meshLoadingRepository) {}

    virtual std::string createStateRepresentation() override;
    virtual void restoreFromStateRepresentation(const std::string stateRepresentation) override;

    virtual void update(float dt) override;

private:
    enum class DimensionType {
        UNDEFINED, WIDTH, HEIGHT
    };

    float parseNumber(const nlohmann::json& jsonValue, DimensionType dimensionType);
};


#endif //GAME_RENDERINGENGINEDEVSCENE_H
