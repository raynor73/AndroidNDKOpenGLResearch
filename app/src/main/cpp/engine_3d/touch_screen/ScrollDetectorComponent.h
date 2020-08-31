//
// Created by Igor Lapin on 09/08/2020.
//

#ifndef GAME_SCROLL_DETECTOR_COMPONENT_H
#define GAME_SCROLL_DETECTOR_COMPONENT_H


#include <optional>
#include <engine_3d/GameObjectComponent.h>
#include "TouchEvent.h"

struct ScrollEvent {
    float dx;
    float dy;
};

class ScrollDetectorComponent : public GameObjectComponent {

    std::optional<TouchEvent> m_optionalPrevTouchEvent;
    std::optional<ScrollEvent> m_optionalScrollEvent;

public:
    const std::optional<ScrollEvent>& scrollEvent() const { return m_optionalScrollEvent; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual void update() override;

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;
};


#endif //GAME_SCROLL_DETECTOR_COMPONENT_H
