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

public:
    explicit Scene(std::shared_ptr<TimeProvider> timeProvider, std::shared_ptr<DisplayInfo> displayInfo);
    virtual ~Scene() = default;

    std::shared_ptr<GameObject> rootGameObject() { return m_rootGameObject; }

    void update();
    virtual void update(float dt) = 0;

protected:
    std::shared_ptr<GameObject> m_rootGameObject;
    std::shared_ptr<TimeProvider> m_timeProvider;
    std::shared_ptr<DisplayInfo> m_displayInfo;

private:
    float m_prevTimestamp;
    bool m_hasPrevTimestamp;
};


#endif //GAME_SCENE_H
