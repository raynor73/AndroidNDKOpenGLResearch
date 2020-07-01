//
// Created by Igor Lapin on 06/06/2020.
//

#include <utility>
#include <sstream>
#include <exception>
#include "Scene.h"

Scene::Scene(std::shared_ptr<TimeProvider> timeProvider, std::shared_ptr<DisplayInfo> displayInfo) :
    m_rootGameObject(std::make_shared<GameObject>("root")),
    m_timeProvider(std::move(timeProvider)),
    m_displayInfo(std::move(displayInfo)),
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
