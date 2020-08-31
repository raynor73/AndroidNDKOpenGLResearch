//
// Created by Igor Lapin on 31/07/2020.
//

#include <engine_3d/GameObject.h>
#include <engine_3d/Utils.h>
#include "ClickDetectorComponent.h"

using namespace Engine3D::Utils;

const std::string ClickDetectorComponent::TYPE_NAME = "ClickDetectorComponent";

void ClickDetectorComponent::update() {
    GameObjectComponent::update();

    throwErrorIfNoGameObject();

    if (!m_isEnabled || !m_gameObject->isEnabled()) {
        m_prevTouchEventOptional.reset();
        m_isPressDetected = false;
        m_isClickDetected = false;
        return;
    }

    auto m_gestureConsumer = std::static_pointer_cast<GestureConsumerComponent>(
            m_gameObject->findComponent(GestureConsumerComponent::TYPE_NAME)
    );

    throwErrorIfNull(m_gestureConsumer, "No gesture consumer for click detector");

    m_isClickDetected = false;

    auto touchEvents = m_gestureConsumer->touchEvents();

    if (touchEvents.empty()) {
        return;
    }

    for (auto& touchEvent : touchEvents) {
        if (
                m_prevTouchEventOptional &&
                (touchEvent.type == TouchEventType::UP || touchEvent.type == TouchEventType::CANCEL)
        ) {
            m_prevTouchEventOptional.reset();
            if (
                    touchEvent.x > m_gestureConsumer->left() &&
                    touchEvent.x < m_gestureConsumer->right() &&
                    touchEvent.y > m_gestureConsumer->bottom() &&
                    touchEvent.y < m_gestureConsumer->top()
            ) {
                m_isClickDetected = true;
            }
        } else {
            m_prevTouchEventOptional = touchEvent;
        }

        m_isPressDetected = !(touchEvent.type == TouchEventType::UP || touchEvent.type == TouchEventType::CANCEL);
    }
}

std::shared_ptr<GameObjectComponent> ClickDetectorComponent::clone() {
    auto clone = std::make_shared<ClickDetectorComponent>();
    clone->setEnabled(m_isEnabled);
    return clone;
}
