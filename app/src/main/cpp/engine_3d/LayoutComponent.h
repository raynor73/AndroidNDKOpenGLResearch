//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_LAYOUT_COMPONENT_H
#define ENGINE_3D_LAYOUT_COMPONENT_H


#include <string>
#include <utility>
#include "GameObjectComponent.h"
#include "LayoutParams.h"

class LayoutComponent : public GameObjectComponent {

    LayoutParams m_layoutParams;

    int m_prevReferenceLeftViewBound = INT_MIN;
    int m_prevReferenceTopViewBound = INT_MIN;
    int m_prevReferenceRightViewBound = INT_MIN;
    int m_prevReferenceBottomViewBound = INT_MIN;

public:
    LayoutComponent(LayoutParams layoutParams) : m_layoutParams(std::move(layoutParams)) {}

    virtual const std::string& typeName() const { return TYPE_NAME; };

    virtual void update() override;

    const LayoutParams& layoutParams() const { return m_layoutParams; }
    void setLayoutParams(const LayoutParams& layoutParams) { m_layoutParams = layoutParams; }

    virtual std::shared_ptr <GameObjectComponent> clone();

    static const std::string TYPE_NAME;
};


#endif //ENGINE_3D_LAYOUT_COMPONENT_H
