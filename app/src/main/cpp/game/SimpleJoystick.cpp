//
// Created by Igor Lapin on 09/08/2020.
//

#include <main/L.h>
#include <sstream>
#include "SimpleJoystick.h"

void SimpleJoystick::update() {
    if (isDisplayInfoUpdated()) {
        m_width = m_unitsConverter->complexValueToPixels(m_widthComplexValue);
        m_height = m_unitsConverter->complexValueToPixels(m_heightComplexValue);
        m_isDragging = false;
        m_position = { 0, 0 };
    }

    for (auto& touchEvent : m_gestureConsumer->touchEvents()) {
        if (m_isDragging) {
            if (touchEvent.type == TouchEventType::UP || touchEvent.type == TouchEventType::CANCEL) {
                m_isDragging = false;
                m_position = { 0, 0 };
            } else {
                float dx = touchEvent.x - m_draggingStartX;
                float dy = touchEvent.y - m_draggingStartY;

                float halfWidth = m_width / 2;
                float halfHeight = m_height / 2;

                if (dx > halfWidth) {
                    dx = halfWidth;
                } else if (dx < -halfWidth) {
                    dx = -halfWidth;
                }

                if (dy > halfHeight) {
                    dy = halfHeight;
                } else if (dy < -halfHeight) {
                    dy = -halfHeight;
                }

                m_position = { dx / halfWidth, dy / halfHeight };
            }
        } else {
            if (touchEvent.type == TouchEventType::DOWN) {
                m_isDragging = true;
                m_draggingStartX = touchEvent.x;
                m_draggingStartY = touchEvent.y;
            }
        }
    }
}
