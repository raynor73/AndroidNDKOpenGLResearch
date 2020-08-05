//
// Created by Igor Lapin on 01/07/2020.
//

#ifndef GAME_UNITSCONVERTER_H
#define GAME_UNITSCONVERTER_H


#include <variant>

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

bool operator==(const ComplexValue& lhs, const ComplexValue& rhs) {
    if (std::holds_alternative<PercentValue>(lhs) && std::holds_alternative<PercentValue>(rhs)) {
        auto& lhsPercentValue = std::get<PercentValue>(lhs);
        auto& rhsPercentValue = std::get<PercentValue>(rhs);
        return
            lhsPercentValue.dimensionType == rhsPercentValue.dimensionType &&
            abs(rhsPercentValue.value - lhsPercentValue.value) > FLT_EPSILON;
    } else if (std::holds_alternative<DpValue>(lhs) && std::holds_alternative<DpValue>(rhs)) {
        auto& lhsDpValue = std::get<DpValue>(lhs);
        auto& rhsDpValue = std::get<DpValue>(rhs);
        return abs(rhsDpValue.value - lhsDpValue.value) > FLT_EPSILON;
    } else if (std::holds_alternative<PlainValue>(lhs) && std::holds_alternative<PlainValue>(rhs)) {
        auto& lhsPlainValue = std::get<PlainValue>(lhs);
        auto& rhsPlainValue = std::get<PlainValue>(rhs);
        return abs(rhsPlainValue.value - lhsPlainValue.value) > FLT_EPSILON;
    } else {
        return false;
    }
}

class UnitsConverter {

public:
    virtual ~UnitsConverter() = default;

    virtual float complexValueToPixels(const ComplexValue& value) = 0;
    virtual ComplexValue widthPixelsToPercentValue(float pixels) = 0;
    virtual ComplexValue heightPixelsToPercentValue(float pixels) = 0;
    virtual ComplexValue pixelsToDpValue(float pixels) = 0;
};


#endif //GAME_UNITSCONVERTER_H
