//
// Created by Igor Lapin on 27/06/2020.
//

#ifndef ENGINE_3D_CAMERACOMPONENT_H
#define ENGINE_3D_CAMERACOMPONENT_H


#include <vector>
#include <string>
#include <glm/mat4x4.hpp>
#include "GameObjectComponent.h"

class CameraComponent : public GameObjectComponent {

    float m_zNear;
    float m_zFar;

    float m_viewportX = 0;
    float m_viewportY = 0;
    float m_viewportWidth = 1;
    float m_viewportHeight = 1;

    std::vector<std::string> m_layerNames;

public:
    CameraComponent(const std::vector<std::string>& layerNames, float zNear, float zFar) :
        m_layerNames(layerNames),
        m_zNear(zNear),
        m_zFar(zFar) {}

    float zNear() const { return m_zNear; };
    void setZNear(float zNear) { m_zNear = zNear; };
    float zFar() const { return m_zFar; };
    void setZFar(float zFar) { m_zFar = zFar; };

    float viewportX() const { return m_viewportX; };
    void setViewportX(float viewportX) { m_viewportX = viewportX; };
    float viewportY() const { return m_viewportY; };
    void setViewportY(float viewportY) { m_viewportY = viewportY; };
    float viewportWidth() const { return m_viewportWidth; };
    void setViewportWidth(float viewportWidth) { m_viewportWidth = viewportWidth; };
    float viewportHeight() const { return m_viewportHeight; };
    void setViewportHeight(float viewportHeight) { m_viewportHeight = viewportHeight; };

    const std::vector<const std::string>& layerNames() const { return m_layerNames; };
    void setLayerNames(const std::vector<std::string>& layerNames);

    glm::mat4 calculateViewMatrix()
};


#endif //ENGINE_3D_CAMERACOMPONENT_H
