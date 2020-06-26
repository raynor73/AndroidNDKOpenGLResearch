//
// Created by Igor Lapin on 02/06/2020.
//

#include <utility>
#include "GameObject.h"

using namespace std;

GameObject::GameObject(std::string name) : m_name(std::move(name)), m_isEnabled(true) {}

GameObject::~GameObject() {
    m_children.clear();
    m_components.clear();
}

void GameObject::addChild(std::shared_ptr<GameObject> child) {
    auto childName = child->name();
    if (m_children.count(childName) > 0) {
        std::stringstream ss;
        ss << "Trying to add " << childName << " game object second time";
        throw std::domain_error(ss.str());
    }

    m_children[childName] = child;
    child->m_parent = std::shared_ptr<GameObject>(this);
    child->onAttachedToParent();
}

void GameObject::removeChild(std::shared_ptr<GameObject> child) {
    auto childName = child->name();
    if (m_children.count(childName) == 0) {
        std::stringstream ss;
        ss << childName << " game object not found";
        throw std::domain_error(ss.str());
    }

    m_children.erase(childName);
    child->m_parent.reset();
    child->onDetachedFromParent();
}

void GameObject::addComponent(std::shared_ptr<GameObjectComponent> component) {
    auto componentName = component->name();
    if (m_components.count(componentName) > 0) {
        std::stringstream ss;
        ss << "Already have " << componentName << " component";
        throw std::domain_error(ss.str());
    }

    m_components[componentName] = component;
    component->setGameObject(std::shared_ptr<GameObject>(this));
    component->onAttachedToGameObject();
}

void GameObject::removeComponent(std::shared_ptr<GameObjectComponent> component) {
    auto componentName = component->name();
    if (m_components.count(componentName) == 0) {
        std::stringstream ss;
        ss << "Component " << componentName << " not found";
        throw std::domain_error(ss.str());
    }

    m_components.erase(componentName);
    component->setGameObject(std::shared_ptr<GameObject>());
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
    // do nothing
}

void GameObject::onDetachedFromParent() {
    // do nothing
}

std::shared_ptr<GameObjectComponent> GameObject::findComponent(const std::string &name) {
    return m_components[name];
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
