//
// Created by Igor Lapin on 02/06/2020.
//

#include <utility>
#include <main/L.h>
#include <sstream>
#include "GameObject.h"

using namespace std;

GameObject::GameObject(std::string name) : m_name(std::move(name)), m_isEnabled(true) {}

GameObject::~GameObject() {
    for (auto& entry : m_children) {
        removeChild(entry.second);
    }
    for (auto& entry : m_components) {
        removeComponent(entry.second);
    }
}

void GameObject::addChild(const std::shared_ptr<GameObject>& child) {
    auto childName = child->name();
    if (m_children.count(childName) > 0) {
        std::stringstream ss;
        ss << "Trying to add " << childName << " game object second time";
        throw std::domain_error(ss.str());
    }

    m_children[childName] = child;
    child->m_parent = this;
    child->onAttachedToParent();
}

void GameObject::removeChild(const std::shared_ptr<GameObject>& child) {
    auto childName = child->name();
    if (m_children.count(childName) == 0) {
        std::stringstream ss;
        ss << childName << " game object not found";
        throw std::domain_error(ss.str());
    }

    m_children.erase(childName);
    child->m_parent = nullptr;
    child->onDetachedFromParent();
}

void GameObject::addComponent(const std::shared_ptr<GameObjectComponent>& component) {
    auto componentName = component->typeName();
    if (m_components.count(componentName) > 0) {
        std::stringstream ss;
        ss << "Already have " << componentName << " component";
        throw std::domain_error(ss.str());
    }

    m_components[componentName] = component;
    component->setGameObject(this);
    component->onAttachedToGameObject();
}

void GameObject::removeComponent(const std::shared_ptr<GameObjectComponent>& component) {
    auto componentName = component->typeName();
    if (m_components.count(componentName) == 0) {
        std::stringstream ss;
        ss << "Component " << componentName << " not found";
        throw std::domain_error(ss.str());
    }

    m_components.erase(componentName);
    component->setGameObject(nullptr);
    component->onDetachedFromGameObject();
}

void GameObject::update() {
    if (m_isEnabled) {
        for (auto &entry : m_components) {
            entry.second->update();
        }

        for (auto &entry : m_children) {
            entry.second->update();
        }
    }
}

void GameObject::onAttachedToParent() {
    for (auto& component : m_components) {
        component.second->onGameObjectAttachedToParent();
    }
}

void GameObject::onDetachedFromParent() {
    for (auto& component : m_components) {
        component.second->onGameObjectDetachedFromParent();
    }
}

std::shared_ptr<GameObjectComponent> GameObject::findComponent(const std::string &name) {
    if (m_components.count(name) > 0) {
        return m_components.at(name);
    } else {
        return nullptr;
    }
}

std::shared_ptr<GameObject> GameObject::clone() {
    std::stringstream ss;
    ss << m_name << nextCloneNamePostfix();
    return clone(ss.str());
}

std::shared_ptr<GameObject> GameObject::clone(const std::string &cloneName) {
    auto clone = new GameObject(cloneName);

    for (auto& entry : m_children) {
        clone->addChild(entry.second->clone());
    }

    for (auto& entry : m_components) {
        clone->addComponent(entry.second->clone());
    }

    return std::shared_ptr<GameObject>(clone);
}

int GameObject::s_cloneNamePostfix = 0;

int GameObject::nextCloneNamePostfix() {
    return s_cloneNamePostfix++;
}
