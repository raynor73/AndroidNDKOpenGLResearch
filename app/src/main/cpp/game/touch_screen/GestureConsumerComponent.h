//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_GESTURE_CONSUMER_COMPONENT_H
#define ENGINE_3D_GESTURE_CONSUMER_COMPONENT_H


#include <string>
#include <vector>
#include <engine_3d/GameObjectComponent.h>
#include <engine_3d/ViewBoundsComponent.h>
#include "TouchEvent.h"

class GestureConsumerComponent : public GameObjectComponent {

    int m_priority;
    std::vector<TouchEvent> m_touchEvents;

public:
    GestureConsumerComponent(uint priority) : m_priority(priority) {}

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    int priority() const { return m_priority; }
    void setPriority(int priority) { m_priority = priority; }

    int left() const;
    int top() const;
    int right() const;
    int bottom() const;

    void onTouchEvent(const TouchEvent& touchEvent);

    const std::vector<TouchEvent>& touchEvents() const { return m_touchEvents; }
    void prepare();

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;

private:
    std::shared_ptr<ViewBoundsComponent> findViewBounds() const;
};


#endif //ENGINE_3D_GESTURE_CONSUMER_COMPONENT_H
