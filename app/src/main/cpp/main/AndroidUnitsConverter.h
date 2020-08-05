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

    virtual float complexValueToPixels(const ComplexValue& value) override;
    virtual ComplexValue widthPixelsToPercentValue(float pixels) override;
    virtual ComplexValue heightPixelsToPercentValue(float pixels) override;
    virtual ComplexValue pixelsToDpValue(float pixels) override;
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDUNITSCONVERTER_H
