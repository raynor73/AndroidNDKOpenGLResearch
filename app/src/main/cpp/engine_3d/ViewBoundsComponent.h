//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_VIEW_BOUNDS_COMPONENT_H
#define ENGINE_3D_VIEW_BOUNDS_COMPONENT_H


#include <string>
#include "GameObjectComponent.h"

class ViewBoundsComponent : public GameObjectComponent {

    uint m_left;
    uint m_top;
    uint m_right;
    uint m_bottom;

public:
    ViewBoundsComponent(
            uint left,
            uint top,
            uint right,
            uint bottom
    ) : m_left(left),
        m_top(top),
        m_right(right),
        m_bottom(bottom) {}

    uint left() const { return m_left; }
    void setLeft(uint left) { m_left = left; }

    uint top() const { return m_top; }
    void setTop(uint top) { m_top = top; }

    uint right() const { return m_right; }
    void setRight(uint right) { m_right = right; }

    uint bottom() const { return m_bottom; }
    void setBottom(uint bottom) { m_bottom = bottom; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;
};


#endif //ENGINE_3D_VIEW_BOUNDS_COMPONENT_H
