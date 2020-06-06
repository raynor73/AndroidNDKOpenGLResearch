//
// Created by Igor Lapin on 06/06/2020.
//

#include "Scene.h"

#include <utility>

Scene::Scene(std::shared_ptr<TimeProvider> timeProvider) :
    m_rootGameObject(std::make_shared<GameObject>("root")),
    m_timeProvider(std::move(timeProvider)),
    m_prevTimestamp(0.0f),
    m_hasPrevTimestamp(false)
{
    // do nothing
}

void Scene::update() {
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
