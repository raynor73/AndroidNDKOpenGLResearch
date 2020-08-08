//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_VIEW_BOUNDS_COMPONENT_H
#define ENGINE_3D_VIEW_BOUNDS_COMPONENT_H


#include <string>
#include <memory>
#include <utility>
#include <variant>
#include <game/UnitsConverter.h>
#include <game/DisplayInfoUpdateDetector.h>
#include "GameObjectComponent.h"

struct EdgesInitParams {
    ComplexValue leftComplexValue;
    ComplexValue topComplexValue;
    ComplexValue rightComplexValue;
    ComplexValue bottomComplexValue;
};

struct SizeInitParams {
    ComplexValue leftComplexValue;
    ComplexValue bottomComplexValue;
    ComplexValue widthComplexValue;
    ComplexValue heightComplexValue;
};

class ViewBoundsComponent : public GameObjectComponent, DisplayInfoUpdateDetector {

    std::shared_ptr<UnitsConverter> m_unitsConverter;

    std::variant<EdgesInitParams, SizeInitParams> m_initParams;

    int m_left;
    int m_top;
    int m_right;
    int m_bottom;

public:
    ViewBoundsComponent(
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            EdgesInitParams initParams
    );

    ViewBoundsComponent(
            std::shared_ptr<DisplayInfo> displayInfo,
            std::shared_ptr<UnitsConverter> unitsConverter,
            SizeInitParams initParams
    );

    int left() const { return m_left; }
    void setLeft(int left) { m_left = left; }

    int top() const { return m_top; }
    void setTop(int top) { m_top = top; }

    int right() const { return m_right; }
    void setRight(int right) { m_right = right; }

    int bottom() const { return m_bottom; }
    void setBottom(int bottom) { m_bottom = bottom; }

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    // TODO May be it makes sense to make checks in getters and/or setters instead of doing them in update
    virtual void update() override;

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    static const std::string TYPE_NAME;

private:
    void calculateBounds();
};


#endif //ENGINE_3D_VIEW_BOUNDS_COMPONENT_H
