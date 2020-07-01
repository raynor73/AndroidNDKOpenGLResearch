//
// Created by Igor Lapin on 01/07/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDUNITSCONVERTER_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDUNITSCONVERTER_H


#include <game/UnitsConverter.h>
#include <game/DisplayInfo.h>
#include <memory>

class AndroidUnitsConverter : public UnitsConverter {

    std::shared_ptr<DisplayInfo> m_displayInfo;

public:
    AndroidUnitsConverter(std::shared_ptr<DisplayInfo> displayInfo) : m_displayInfo(displayInfo) {}

    virtual float widthPercentToPixels(float percentValue) override;
    virtual float widthPixelsToPercent(float pixels) override;
    virtual float heightPercentToPixels(float percentValue) override;
    virtual float heightPixelsToPercent(float pixels) override;

    virtual float dpToPixels(float dpValue) override;
    virtual float pixelsToDp(float pixels) override;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDUNITSCONVERTER_H
