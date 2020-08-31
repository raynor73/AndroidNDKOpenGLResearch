//
// Created by Igor Lapin on 31/07/2020.
//

#ifndef GAME_CLICK_DETECTOR_H
#define GAME_CLICK_DETECTOR_H


#include <memory>
#include <engine_3d/GameObjectComponent.h>
#include <optional>
#include "TouchEvent.h"
#include "GestureConsumerComponent.h"

class ClickDetectorComponent : public GameObjectComponent {

    std::optional<TouchEvent> m_prevTouchEventOptional;

    bool m_isClickDetected = false;
    bool m_isPressDetected = false;

public:
    bool isClickDetected() const { return m_isClickDetected; }
    bool isPressDetected() const { return m_isPressDetected; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual void update() override;

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;
};


#endif //GAME_CLICK_DETECTOR_H
