//
// Created by Igor Lapin on 26/07/2020.
//

#include <memory>
#include <exception>
#include <game/touch_screen/GesturesDispatcher.h>
#include "engine_3d/GameObject.h"
#include "GestureConsumerComponent.h"

const std::string GestureConsumerComponent::TYPE_NAME = "GestureConsumerComponent";

std::shared_ptr<GameObjectComponent> GestureConsumerComponent::clone() {
    auto clone = std::make_shared<GestureConsumerComponent>(m_gesturesDispatcher, m_priority);
    clone->setEnabled(m_isEnabled);
    m_gesturesDispatcher->addGestureConsumer(clone);
    return clone;
}

int GestureConsumerComponent::left() const {
    return findViewBounds()->left();
}

int GestureConsumerComponent::top() const {
    return findViewBounds()->top();
}

int GestureConsumerComponent::right() const {
    return findViewBounds()->right();
}

int GestureConsumerComponent::bottom() const {
    return findViewBounds()->bottom();
}

void GestureConsumerComponent::onTouchEvent(const TouchEvent& touchEvent) {
    m_touchEvents.push_back(touchEvent);
}

void GestureConsumerComponent::prepare() {
    m_touchEvents.clear();
}

std::shared_ptr<ViewBoundsComponent> GestureConsumerComponent::findViewBounds() const {
    throwErrorIfNoGameObject();

    auto viewBounds = std::static_pointer_cast<ViewBoundsComponent>(
            m_gameObject->findComponent(ViewBoundsComponent::TYPE_NAME)
    );
    if (viewBounds == nullptr) {
        throw std::domain_error("No View Bounds found for Gesture Consumer");
    }

    return viewBounds;
}
