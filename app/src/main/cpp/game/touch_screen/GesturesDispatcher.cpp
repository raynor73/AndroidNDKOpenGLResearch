//
// Created by Igor Lapin on 26/07/2020.
//

#include <exception>
#include <algorithm>
#include "GesturesDispatcher.h"

void GesturesDispatcher::addGestureConsumer(std::shared_ptr<GestureConsumerComponent> gestureConsumer) {
    if (m_gestureConsumers.count(gestureConsumer) > 0) {
        throw std::domain_error("Trying to add gesture consumer twice");
    }

    m_gestureConsumers.insert(gestureConsumer);
}

void GesturesDispatcher::removeGestureConsumer(std::shared_ptr<GestureConsumerComponent> gestureConsumer) {
    if (m_gestureConsumers.count(gestureConsumer) == 0) {
        throw std::domain_error("Gesture consumer not found");
    }

    m_gestureConsumers.erase(gestureConsumer);

    if (m_gestureOwnerToEventIdMap.count(gestureConsumer) > 0) {
        auto eventId = m_gestureOwnerToEventIdMap.at(gestureConsumer);
        m_gestureOwnerToEventIdMap.erase(gestureConsumer);
        m_eventIdToGestureOwnerMap.erase(eventId);
    }
}

void GesturesDispatcher::removeAllGestureConsumers() {
    m_gestureConsumers.clear();
    m_gestureOwnerToEventIdMap.clear();
    m_eventIdToGestureOwnerMap.clear();
}

void GesturesDispatcher::prepare() {
    for (auto& gestureConsumer : m_gestureConsumers) {
        gestureConsumer->prepare();
    }
}

void GesturesDispatcher::onTouchEvent(const TouchEvent& touchEvent) {
    if (m_eventIdToGestureOwnerMap.count(touchEvent.id) > 0) {
        m_eventIdToGestureOwnerMap.at(touchEvent.id)->onTouchEvent(touchEvent);
    } else if (touchEvent.type == TouchEventType::DOWN) {
        auto matchingConsumer = findMatchingGestureConsumer(touchEvent);
        if (matchingConsumer != nullptr) {
            m_eventIdToGestureOwnerMap.insert({touchEvent.id, matchingConsumer});
            m_gestureOwnerToEventIdMap.insert({matchingConsumer, touchEvent.id});
            matchingConsumer->onTouchEvent(touchEvent);
        }
    }

    if (touchEvent.type == TouchEventType::CANCEL || touchEvent.type == TouchEventType::UP) {
        if (m_eventIdToGestureOwnerMap.count(touchEvent.id) > 0) {
            auto gestureOwner = m_eventIdToGestureOwnerMap.at(touchEvent.id);
            m_gestureOwnerToEventIdMap.erase(gestureOwner);
            m_eventIdToGestureOwnerMap.erase(touchEvent.id);
        }
    }
}

std::shared_ptr<GestureConsumerComponent> GesturesDispatcher::findMatchingGestureConsumer(
        const TouchEvent& touchEvent
) {
    for (auto& gestureConsumer : m_gestureConsumers) {
        if (
                touchEvent.x >= gestureConsumer->left() && touchEvent.x <= gestureConsumer->right() &&
                touchEvent.y >= gestureConsumer->bottom() && touchEvent.y <= gestureConsumer->top()
        ) {
            return gestureConsumer;
        }
    }

    return nullptr;
}


