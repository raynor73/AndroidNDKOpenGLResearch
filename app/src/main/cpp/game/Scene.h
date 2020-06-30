//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef GAME_SCENE_H
#define GAME_SCENE_H


#include <memory>
#include <engine_3d/GameObject.h>
#include "TimeProvider.h"
#include "DisplayInfo.h"

class Scene {

    float m_prevTimestamp;
    bool m_hasPrevTimestamp;

public:
    explicit Scene(std::shared_ptr<TimeProvider> timeProvider, std::shared_ptr<DisplayInfo> displayInfo);
    virtual ~Scene() = default;

    std::shared_ptr<GameObject> rootGameObject() { return m_rootGameObject; }

    virtual std::string createStateRepresentation() = 0;
    virtual void restoreFromStateRepresentation(const std::string stateRepresentation) = 0;

    void update();

protected:
    std::shared_ptr<GameObject> m_rootGameObject;
    std::shared_ptr<TimeProvider> m_timeProvider;
    std::shared_ptr<DisplayInfo> m_displayInfo;

    virtual void update(float dt) = 0;
};


#endif //GAME_SCENE_H
