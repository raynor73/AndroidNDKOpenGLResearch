//
// Created by Igor Lapin on 26/07/2020.
//

#include <memory>
#include <sstream>
#include <exception>
#include "GameObject.h"
#include "LayoutComponent.h"
#include "ViewBoundsComponent.h"
#include "TransformationComponent.h"

const std::string LayoutComponent::TYPE_NAME = "LayoutComponent";

void LayoutComponent::update() {
    GameObjectComponent::update();

    if (!m_isEnabled) {
        return;
    }

    if (m_layoutParams.referenceViewBounds == nullptr) {
        throw std::domain_error("Can't layout without reference view bounds provided");
    }
    auto viewBounds = std::static_pointer_cast<ViewBoundsComponent>(
            m_gameObject->findComponent(ViewBoundsComponent::TYPE_NAME)
    );
    if (viewBounds == nullptr) {
        throw std::domain_error("No View Bounds to layout");
    }

    int currentReferenceLeftViewBound = m_layoutParams.referenceViewBounds->left();
    int currentReferenceTopViewBound = m_layoutParams.referenceViewBounds->top();
    int currentReferenceRightViewBound = m_layoutParams.referenceViewBounds->right();
    int currentReferenceBottomViewBound = m_layoutParams.referenceViewBounds->bottom();
    if (
            currentReferenceLeftViewBound == m_prevReferenceLeftViewBound &&
            currentReferenceTopViewBound == m_prevReferenceTopViewBound &&
            currentReferenceRightViewBound == m_prevReferenceRightViewBound &&
            currentReferenceBottomViewBound == m_prevReferenceBottomViewBound &&
            m_calculatedLeftViewBound == viewBounds->left() &&
            m_calculatedTopViewBound == viewBounds->top() &&
            m_calculatedRightViewBound == viewBounds->right() &&
            m_calculatedBottomViewBound == viewBounds->bottom()
    ) {
        return;
    }
    m_prevReferenceLeftViewBound = currentReferenceLeftViewBound;
    m_prevReferenceTopViewBound = currentReferenceTopViewBound;
    m_prevReferenceRightViewBound = currentReferenceRightViewBound;
    m_prevReferenceBottomViewBound = currentReferenceBottomViewBound;

    throwErrorIfNoGameObject();

    auto width = viewBounds->right() - viewBounds->left();
    auto height = viewBounds->top() - viewBounds->bottom();
    
    auto paddingLeft = m_unitsConverter->complexValueToPixels(m_layoutParams.paddingLeft);
    auto paddingTop = m_unitsConverter->complexValueToPixels(m_layoutParams.paddingTop);
    auto paddingRight = m_unitsConverter->complexValueToPixels(m_layoutParams.paddingRight);
    auto paddingBottom = m_unitsConverter->complexValueToPixels(m_layoutParams.paddingBottom);

    switch (m_layoutParams.horizontalLayoutType) {

        case HorizontalLayoutType::LEFT_INSIDE:
            m_calculatedLeftViewBound = currentReferenceLeftViewBound + paddingLeft;
            m_calculatedRightViewBound = m_calculatedLeftViewBound + width;
            viewBounds->setLeft(m_calculatedLeftViewBound);
            viewBounds->setRight(m_calculatedRightViewBound);
            break;

        case HorizontalLayoutType::LEFT_OUTSIDE:
            m_calculatedRightViewBound = currentReferenceLeftViewBound - paddingRight;
            m_calculatedLeftViewBound = m_calculatedRightViewBound - width;
            viewBounds->setRight(m_calculatedRightViewBound);
            viewBounds->setLeft(m_calculatedLeftViewBound);
            break;

        case HorizontalLayoutType::CENTER: {
            auto referenceCenter = (currentReferenceLeftViewBound + currentReferenceRightViewBound) / 2;
            auto halfWidth = width / 2;
            m_calculatedLeftViewBound = referenceCenter - halfWidth + paddingLeft;
            m_calculatedRightViewBound = m_calculatedLeftViewBound + width;
            viewBounds->setLeft(m_calculatedLeftViewBound);
            viewBounds->setRight(m_calculatedRightViewBound);
            break;
        }

        case HorizontalLayoutType::RIGHT_INSIDE:
            m_calculatedRightViewBound = currentReferenceRightViewBound - paddingRight;
            m_calculatedLeftViewBound = m_calculatedRightViewBound - width;
            viewBounds->setRight(m_calculatedRightViewBound);
            viewBounds->setLeft(m_calculatedLeftViewBound);
            break;

        case HorizontalLayoutType::RIGHT_OUTSIDE:
            m_calculatedLeftViewBound = currentReferenceRightViewBound + paddingLeft;
            m_calculatedRightViewBound = m_calculatedLeftViewBound + width;
            viewBounds->setLeft(m_calculatedLeftViewBound);
            viewBounds->setRight(m_calculatedRightViewBound);
            break;
    }

    switch (m_layoutParams.verticalLayoutType) {

        case VerticalLayoutType::TOP_INSIDE:
            m_calculatedTopViewBound = currentReferenceTopViewBound - paddingTop;
            m_calculatedBottomViewBound = m_calculatedTopViewBound - height;
            viewBounds->setTop(m_calculatedTopViewBound);
            viewBounds->setBottom(m_calculatedBottomViewBound);
            break;

        case VerticalLayoutType::TOP_OUTSIDE:
            m_calculatedBottomViewBound = currentReferenceBottomViewBound + paddingBottom;
            m_calculatedTopViewBound = m_calculatedBottomViewBound + height;
            viewBounds->setBottom(m_calculatedBottomViewBound);
            viewBounds->setTop(m_calculatedTopViewBound);
            break;

        case VerticalLayoutType::CENTER: {
            auto referenceCenter = (currentReferenceTopViewBound + currentReferenceBottomViewBound) / 2;
            auto halfHeight = height / 2;
            m_calculatedBottomViewBound = referenceCenter - halfHeight + paddingBottom;
            m_calculatedTopViewBound = m_calculatedBottomViewBound + height;
            viewBounds->setBottom(m_calculatedBottomViewBound);
            viewBounds->setTop(m_calculatedTopViewBound);
            break;
        }

        case VerticalLayoutType::BOTTOM_INSIDE:
            m_calculatedBottomViewBound = currentReferenceBottomViewBound + paddingBottom;
            m_calculatedTopViewBound = m_calculatedBottomViewBound + height;
            viewBounds->setBottom(m_calculatedBottomViewBound);
            viewBounds->setTop(m_calculatedTopViewBound);
            break;

        case VerticalLayoutType::BOTTOM_OUTSIDE:
            m_calculatedTopViewBound = currentReferenceBottomViewBound - paddingTop;
            m_calculatedBottomViewBound = m_calculatedTopViewBound - height;
            viewBounds->setTop(m_calculatedTopViewBound);
            viewBounds->setBottom(m_calculatedBottomViewBound);
            break;
    }

    float originPositionY;
    switch (m_layoutParams.originVerticalLayoutType) {

        case OriginVerticalLayoutType::TOP:
            originPositionY = viewBounds->top();
            break;

        case OriginVerticalLayoutType::CENTER:
            originPositionY = (viewBounds->top() + viewBounds->bottom()) / 2;
            break;

        case OriginVerticalLayoutType::BOTTOM:
            originPositionY = viewBounds->bottom();
            break;
    }

    float originPositionX;
    switch (m_layoutParams.originHorizontalLayoutType) {

        case OriginHorizontalLayoutType::LEFT:
            originPositionX = viewBounds->left();
            break;

        case OriginHorizontalLayoutType::CENTER:
            originPositionX = (viewBounds->right() + viewBounds->left()) / 2;
            break;

        case OriginHorizontalLayoutType::RIGHT:
            originPositionX = viewBounds->right();
            break;
    }

    auto transform = std::static_pointer_cast<TransformationComponent>(
            m_gameObject->findComponent(TransformationComponent::TYPE_NAME)
    );
    if (transform == nullptr) {
        throw std::domain_error("No transform found for laying out game object");
    }

    auto originPosition = transform->position();
    originPosition.x = originPositionX;
    originPosition.y = originPositionY;
    transform->setPosition(originPosition);
}

std::shared_ptr <GameObjectComponent> LayoutComponent::clone() {
    auto clone = std::make_shared<LayoutComponent>(m_layoutParams, m_unitsConverter);
    clone->setEnabled(m_isEnabled);
    return clone;
}
