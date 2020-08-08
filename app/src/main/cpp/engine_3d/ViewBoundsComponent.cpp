//
// Created by Igor Lapin on 26/07/2020.
//

#include <memory>
#include "ViewBoundsComponent.h"

const std::string ViewBoundsComponent::TYPE_NAME = "ViewBoundsComponent";

ViewBoundsComponent::ViewBoundsComponent(
        std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<UnitsConverter> unitsConverter,
        EdgesInitParams initParams
) : DisplayInfoUpdateDetector(std::move(displayInfo)),
    m_unitsConverter(std::move(unitsConverter)),
    m_initParams(std::move(initParams))
{
    calculateBounds();
}

ViewBoundsComponent::ViewBoundsComponent(
        std::shared_ptr<DisplayInfo> displayInfo,
        std::shared_ptr<UnitsConverter> unitsConverter,
        SizeInitParams initParams
) : DisplayInfoUpdateDetector(std::move(displayInfo)),
    m_unitsConverter(std::move(unitsConverter)),
    m_initParams(initParams)
{
    calculateBounds();
}

std::shared_ptr<GameObjectComponent> ViewBoundsComponent::clone() {
    std::shared_ptr<GameObjectComponent> clone;
    if (std::holds_alternative<EdgesInitParams>(m_initParams)) {
        auto viewBounds = std::get<EdgesInitParams>(m_initParams);
        clone = std::make_shared<ViewBoundsComponent>(
                m_displayInfo,
                m_unitsConverter,
                viewBounds
        );
    } else {
        auto viewBounds = std::get<SizeInitParams>(m_initParams);
        clone = std::make_shared<ViewBoundsComponent>(
                m_displayInfo,
                m_unitsConverter,
                viewBounds
        );
    }
    clone->setEnabled(m_isEnabled);
    return clone;
}

void ViewBoundsComponent::update() {
    GameObjectComponent::update();

    if (!m_isEnabled) {
        return;
    }

    if (isDisplayInfoUpdated()) {
        calculateBounds();
    }
}

void ViewBoundsComponent::calculateBounds() {
    if (std::holds_alternative<EdgesInitParams>(m_initParams)) {
        auto viewBounds = std::get<EdgesInitParams>(m_initParams);
        m_left = m_unitsConverter->complexValueToPixels(viewBounds.leftComplexValue);
        m_top = m_unitsConverter->complexValueToPixels(viewBounds.topComplexValue);
        m_right = m_unitsConverter->complexValueToPixels(viewBounds.rightComplexValue);
        m_bottom = m_unitsConverter->complexValueToPixels(viewBounds.bottomComplexValue);
    } else {
        auto viewBounds = std::get<SizeInitParams>(m_initParams);
        m_left = m_unitsConverter->complexValueToPixels(viewBounds.leftComplexValue);
        m_top =
                m_unitsConverter->complexValueToPixels(viewBounds.bottomComplexValue) +
                m_unitsConverter->complexValueToPixels(viewBounds.heightComplexValue);
        m_right =
                m_unitsConverter->complexValueToPixels(viewBounds.leftComplexValue) +
                m_unitsConverter->complexValueToPixels(viewBounds.widthComplexValue);
        m_bottom = m_unitsConverter->complexValueToPixels(viewBounds.bottomComplexValue);
    }
}
