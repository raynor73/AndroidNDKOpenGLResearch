//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_VIEW_BOUNDS_COMPONENT_H
#define ENGINE_3D_VIEW_BOUNDS_COMPONENT_H


#include <string>
#include "GameObjectComponent.h"

class ViewBoundsComponent : public GameObjectComponent {

    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

public:
    ViewBoundsComponent(
            int left,
            int top,
            int right,
            int bottom
    ) : m_left(left),
        m_top(top),
        m_right(right),
        m_bottom(bottom) {}

    int left() const { return m_left; }
    void setLeft(int left);

    int top() const { return m_top; }
    void setTop(int top);

    int right() const { return m_right; }
    void setRight(int right);

    int bottom() const { return m_bottom; }
    void setBottom(int bottom);

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;

private:
    void requestLayoutIfNecessary();
};


#endif //ENGINE_3D_VIEW_BOUNDS_COMPONENT_H
