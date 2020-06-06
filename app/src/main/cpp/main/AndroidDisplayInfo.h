//
// Created by Igor Lapin on 06/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDDISPLAYINFO_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDDISPLAYINFO_H


#include "../game/DisplayInfo.h"

class AndroidDisplayInfo : public DisplayInfo {

public:
    explicit AndroidDisplayInfo(float initialWidth, float initialHeight, float initialDensityFactor);

    virtual float width() const;
    virtual float height() const;
    virtual float densityFactor() const;

    void setWidth(float width) { m_width = width; }
    void setHeight(float height) { m_height = height; }
    void setDensityFactor(float densityFactor) { m_densityFactor = densityFactor; }

private:
    float m_width;
    float m_height;
    float m_densityFactor;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDDISPLAYINFO_H
