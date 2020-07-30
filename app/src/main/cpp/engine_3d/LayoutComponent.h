//
// Created by Igor Lapin on 26/07/2020.
//

#ifndef ENGINE_3D_LAYOUT_COMPONENT_H
#define ENGINE_3D_LAYOUT_COMPONENT_H


#include <string>
#include <utility>
#include <unordered_set>
#include <memory>
#include "GameObjectComponent.h"
#include "LayoutParams.h"

class LayoutComponent : public GameObjectComponent {

    bool m_isLayoutRequired = true;
    LayoutParams m_layoutParams;
    std::shared_ptr<LayoutComponent> m_parentLayout;
    std::unordered_set<std::shared_ptr<LayoutComponent>> m_childrenLayouts;

public:
    LayoutComponent(LayoutParams layoutParams) : m_layoutParams(std::move(layoutParams)) {}

    virtual const std::string& typeName() const override { return TYPE_NAME; };

    virtual void update() override;

    const LayoutParams& layoutParams() const { return m_layoutParams; }
    void setLayoutParams(const LayoutParams& layoutParams);

    void addChildLayout(const std::shared_ptr<LayoutComponent>& child);
    void removeChildLayout(const std::shared_ptr<LayoutComponent>& child);

    void requestLayout();

    virtual std::shared_ptr <GameObjectComponent> clone();

    static const std::string TYPE_NAME;
};


#endif //ENGINE_3D_LAYOUT_COMPONENT_H
