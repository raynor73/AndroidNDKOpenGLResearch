//
// Created by Igor Lapin on 09/08/2020.
//

#include <engine_3d/Utils.h>
#include <engine_3d/GameObject.h>
#include "GestureConsumerComponent.h"
#include "ScrollDetectorComponent.h"

using namespace Engine3D::Utils;

const std::string ScrollDetectorComponent::TYPE_NAME = "ScrollDetectorComponent";

void ScrollDetectorComponent::update() {
    GameObjectComponent::update();

    if (!m_isEnabled) {
        m_optionalPrevTouchEvent.reset();
        m_optionalScrollEvent.reset();
    }

    m_optionalScrollEvent.reset();

    throwErrorIfNoGameObject();

    auto m_gestureConsumer = std::static_pointer_cast<GestureConsumerComponent>(
            m_gameObject->findComponent(GestureConsumerComponent::TYPE_NAME)
    );

    throwErrorIfNull(m_gestureConsumer, "No gesture consumer for scroll detector");

    auto touchEvents = m_gestureConsumer->touchEvents();

    if (touchEvents.empty()) {
        return;
    }

    for (auto& touchEvent : touchEvents) {
        if (m_optionalPrevTouchEvent) {
            m_optionalScrollEvent = {
                touchEvent.x - m_optionalPrevTouchEvent.value().x,
                touchEvent.y - m_optionalPrevTouchEvent.value().y,
            };
        }

        if (touchEvent.type == TouchEventType::UP || touchEvent.type == TouchEventType::CANCEL) {
            m_optionalPrevTouchEvent.reset();
        } else {
            m_optionalPrevTouchEvent = touchEvent;
        }
    }
}

std::shared_ptr<GameObjectComponent> ScrollDetectorComponent::clone() {
    auto clone = std::make_shared<ScrollDetectorComponent>();
    clone->setEnabled(m_isEnabled);
    return clone;
}

