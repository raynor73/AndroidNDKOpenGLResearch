//
// Created by Igor Lapin on 09/08/2020.
//

#ifndef GAME_SIMPLE_JOYSTICK_H
#define GAME_SIMPLE_JOYSTICK_H


#include <memory>
#include <utility>
#include <string>
#include <game/touch_screen/GestureConsumerComponent.h>
#include <game/DisplayInfoUpdateDetector.h>
#include <game/UnitsConverter.h>
#include "Joystick.h"

class SimpleJoystick : public Joystick, DisplayInfoUpdateDetector {

    std::shared_ptr<GestureConsumerComponent> m_gestureConsumer;
    std::shared_ptr<UnitsConverter> m_unitsConverter;

    JoystickPosition m_position { 0, 0 };
    ComplexValue m_widthComplexValue;
    ComplexValue m_heightComplexValue;

    float m_width;
    float m_height;

    bool m_isDragging = false;
    float m_draggingStartX = NAN;
    float m_draggingStartY = NAN;

    bool m_isEnabled = true;

public:
    SimpleJoystick(
            std::shared_ptr<GestureConsumerComponent> gestureConsumer,
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<DisplayInfo> displayInfo,
            ComplexValue width,
            ComplexValue height
    ) : DisplayInfoUpdateDetector(std::move(displayInfo)),
        m_gestureConsumer(std::move(gestureConsumer)),
        m_unitsConverter(std::move(unitsConverter)),
        m_widthComplexValue(width),
        m_heightComplexValue(height)
    {}

    virtual const JoystickPosition& position() override { return m_position; }

    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    void update();
};


#endif //GAME_SIMPLE_JOYSTICK_H
