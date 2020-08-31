//
// Created by Igor Lapin on 27/06/2020.
//

#ifndef ENGINE_3D_CAMERACOMPONENT_H
#define ENGINE_3D_CAMERACOMPONENT_H


#include <vector>
#include <string>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include "UnitsConverter.h"
#include "GameObjectComponent.h"

class CameraComponent : public GameObjectComponent {

protected:
    float m_zNear;
    float m_zFar;

    ComplexValue m_viewportX = PlainValue { 0 };
    ComplexValue m_viewportY = PlainValue { 0 };
    ComplexValue m_viewportWidth = PercentValue { DimensionType::WIDTH, 100 };
    ComplexValue m_viewportHeight = PercentValue { DimensionType::HEIGHT, 100 };

    int m_order;

    glm::vec4 m_clearColor;
    bool m_shouldClearDepth = true;
    bool m_shouldClearColor = true;

    std::vector<std::string> m_layerNames;

public:
    CameraComponent(
            const glm::vec4& clearColor,
            const std::vector<std::string>& layerNames,
            float zNear,
            float zFar,
            int order
    ) :
        m_clearColor(clearColor),
        m_layerNames(layerNames),
        m_zNear(zNear),
        m_zFar(zFar),
        m_order(order)
        {}

    float zNear() const { return m_zNear; };
    void setZNear(float zNear) { m_zNear = zNear; };
    float zFar() const { return m_zFar; };
    void setZFar(float zFar) { m_zFar = zFar; };

    const ComplexValue& viewportX() const { return m_viewportX; };
    void setViewportX(const ComplexValue& viewportX) { m_viewportX = viewportX; };
    const ComplexValue& viewportY() const { return m_viewportY; };
    void setViewportY(const ComplexValue& viewportY) { m_viewportY = viewportY; };
    const ComplexValue& viewportWidth() const { return m_viewportWidth; };
    void setViewportWidth(const ComplexValue& viewportWidth) { m_viewportWidth = viewportWidth; };
    const ComplexValue& viewportHeight() const { return m_viewportHeight; };
    void setViewportHeight(const ComplexValue& viewportHeight) { m_viewportHeight = viewportHeight; };

    int order() const { return m_order; }
    void setOrder(int priority) { m_order = priority; }

    glm::vec4 clearColor() const { return m_clearColor; }
    void setClearColor(const glm::vec4& clearColor) { m_clearColor = clearColor; }
    bool shouldClearDepth() const { return m_shouldClearDepth; }
    void setShouldClearDepth(bool shouldClearDepth) { m_shouldClearDepth = shouldClearDepth; }
    bool shouldClearColor() const { return m_shouldClearColor; }
    void setShouldClearColor(bool shouldClearColor) { m_shouldClearColor = shouldClearColor; }

    const std::vector<std::string>& layerNames() const { return m_layerNames; };
    void setLayerNames(const std::vector<std::string>& layerNames);

    virtual glm::mat4 calculateViewMatrix() = 0;
    virtual glm::mat4 calculateProjectionMatrix() = 0;
};


#endif //ENGINE_3D_CAMERACOMPONENT_H
