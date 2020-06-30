//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef GAME_RENDERINGENGINEDEVSCENE_H
#define GAME_RENDERINGENGINEDEVSCENE_H


#include <game/Scene.h>

class RenderingEngineDevScene : public Scene {

public:
    RenderingEngineDevScene(
            std::shared_ptr<TimeProvider> timeProvider,
            std::shared_ptr<DisplayInfo> displayInfo
    ) : Scene(timeProvider, displayInfo) {}

    virtual std::string createStateRepresentation() override;
    virtual void restoreFromStateRepresentation(const std::string stateRepresentation) override;

    virtual void update(float dt) override;
};


#endif //GAME_RENDERINGENGINEDEVSCENE_H
