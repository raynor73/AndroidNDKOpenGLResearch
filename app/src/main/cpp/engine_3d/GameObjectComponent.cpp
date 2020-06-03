//
// Created by Igor Lapin on 02/06/2020.
//

#include "GameObjectComponent.h"

GameObjectComponent::GameObjectComponent(const std::string &name) : m_name(name), m_isEnabled(true) {
    // do nothing
}

GameObjectComponent::~GameObjectComponent() {
    // do nothing
}

void GameObjectComponent::update() {
    // do nothing
}

void GameObjectComponent::onAttachedToGameObject() {
    // do nothing
}

void GameObjectComponent::onDetachedFromGameObject() {
    // do nothing
}
