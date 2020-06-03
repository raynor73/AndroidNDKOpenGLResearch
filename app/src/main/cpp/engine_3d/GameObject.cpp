//
// Created by Igor Lapin on 02/06/2020.
//

#include "GameObject.h"

GameObject::GameObject(const std::string &name) : m_name(name), m_isEnabled(true) {}

GameObject::~GameObject() {
    m_children.clear();
    m_components.clear();
}

void GameObject::addChild(GameObject &child) {
    auto childName = child.name();
    if (m_children.count(childName) > 0) {
        std::stringstream ss;
        ss << "Trying to add " << childName << " game object second time";
        throw std::domain_error(ss.str());
    }
    m_children[childName] = std::shared_ptr<GameObject>(&child);
    child.m_parent = std::shared_ptr<GameObject>(this);
}

void GameObject::removeChild(GameObject &child) {
    auto childName = child.name();
    if (m_children.count(childName) == 0) {
        std::stringstream ss;
        ss << childName << " game object not found";
        throw std::domain_error(ss.str());
    }
    m_children.erase(childName);
    child.m_parent.reset();
}

int GameObject::s_cloneNamePostfix = 0;

int GameObject::nextCloneNamePostfix() {
    return s_cloneNamePostfix++;
}

