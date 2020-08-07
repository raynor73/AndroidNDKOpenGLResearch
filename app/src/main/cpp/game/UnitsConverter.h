//
// Created by Igor Lapin on 01/07/2020.
//

#ifndef GAME_UNITSCONVERTER_H
#define GAME_UNITSCONVERTER_H


#include <variant>
#include <ostream>

enum class DimensionType {
    WIDTH, HEIGHT
};

struct PercentValue {
    DimensionType dimensionType;
    float value;
};

struct DpValue {
    float value;
};

struct PlainValue {
    float value;
};

typedef std::variant<PercentValue, DpValue, PlainValue> ComplexValue;

bool operator==(const ComplexValue& lhs, const ComplexValue& rhs);

std::ostream& operator<<(std::ostream& os, const ComplexValue& complexValue);

class UnitsConverter {

public:
    virtual ~UnitsConverter() = default;

    virtual float complexValueToPixels(const ComplexValue& value) = 0;
    virtual ComplexValue widthPixelsToPercentValue(float pixels) = 0;
    virtual ComplexValue heightPixelsToPercentValue(float pixels) = 0;
    virtual ComplexValue pixelsToDpValue(float pixels) = 0;
};


#endif //GAME_UNITSCONVERTER_H
