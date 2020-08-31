//
// Created by Igor Lapin on 07/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_TRANSFORMATION2DCOMPONENT_H
#define ANDROID_NDK_OPENGL_RESEARCH_TRANSFORMATION2DCOMPONENT_H


#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "DisplayInfoUpdateDetector.h"
#include "UnitsConverter.h"
#include "GameObjectComponent.h"

class Transformation2DComponent : public GameObjectComponent, DisplayInfoUpdateDetector {

    std::shared_ptr<UnitsConverter> m_unitsConverter;

    ComplexValue m_positionX;
    ComplexValue m_positionY;
    ComplexValue m_scaleX;
    ComplexValue m_scaleY;

public:
    Transformation2DComponent(
            std::shared_ptr<UnitsConverter> unitsConverter,
            std::shared_ptr<DisplayInfo> displayInfo,
            ComplexValue positionX,
            ComplexValue positionY,
            ComplexValue scaleX,
            ComplexValue scaleY
    ) : DisplayInfoUpdateDetector(displayInfo),
        m_unitsConverter(std::move(unitsConverter)),
        m_positionX(std::move(positionX)),
        m_positionY(std::move(positionY)),
        m_scaleX(std::move(scaleX)),
        m_scaleY(std::move(scaleY))
        {}

    static const std::string TYPE_NAME;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    const ComplexValue& positionX() const { return m_positionX; }
    void setPositionX(const ComplexValue& positionX) { m_positionX = positionX; }
    const ComplexValue& positionY() const { return m_positionY; }
    void setPositionY(const ComplexValue& positionY) { m_positionY = positionY; }
    const ComplexValue& scaleX() const { return m_scaleX; }
    void setScaleX(const ComplexValue& scaleX) { m_scaleX = scaleX; }
    const ComplexValue& scaleY() const { return m_scaleY; }
    void setScaleY(const ComplexValue& scaleY) { m_scaleY = scaleY; }

    virtual void update() override;

    virtual std::shared_ptr <GameObjectComponent> clone() override;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_TRANSFORMATION2DCOMPONENT_H
