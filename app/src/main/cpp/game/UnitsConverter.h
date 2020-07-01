//
// Created by Igor Lapin on 01/07/2020.
//

#ifndef GAME_UNITSCONVERTER_H
#define GAME_UNITSCONVERTER_H


class UnitsConverter {

public:
    UnitsConverter() = default;
    UnitsConverter(const UnitsConverter& other) = delete;
    UnitsConverter(const UnitsConverter&& other) = delete;
    virtual ~UnitsConverter() = default;

    virtual float widthPercentToPixels(float percentValue) = 0;
    virtual float widthPixelsToPercent(float pixels) = 0;
    virtual float heightPercentToPixels(float percentValue) = 0;
    virtual float heightPixelsToPercent(float pixels) = 0;

    virtual float dpToPixels(float dpValue) = 0;
    virtual float pixelsToDp(float pixels) = 0;

    UnitsConverter& operator=(const UnitsConverter& other) = delete;
    UnitsConverter& operator=(UnitsConverter&& other) = delete;
};


#endif //GAME_UNITSCONVERTER_H
