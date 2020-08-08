//
// Created by Igor Lapin on 01/07/2020.
//

#include "UnitsConverter.h"

bool operator==(const ComplexValue& lhs, const ComplexValue& rhs) {
    if (std::holds_alternative<PercentValue>(lhs) && std::holds_alternative<PercentValue>(rhs)) {
        auto& lhsPercentValue = std::get<PercentValue>(lhs);
        auto& rhsPercentValue = std::get<PercentValue>(rhs);
        return
                lhsPercentValue.dimensionType == rhsPercentValue.dimensionType &&
                abs(rhsPercentValue.value - lhsPercentValue.value) < FLT_EPSILON;
    } else if (std::holds_alternative<DpValue>(lhs) && std::holds_alternative<DpValue>(rhs)) {
        auto& lhsDpValue = std::get<DpValue>(lhs);
        auto& rhsDpValue = std::get<DpValue>(rhs);
        return abs(rhsDpValue.value - lhsDpValue.value) < FLT_EPSILON;
    } else if (std::holds_alternative<PlainValue>(lhs) && std::holds_alternative<PlainValue>(rhs)) {
        auto& lhsPlainValue = std::get<PlainValue>(lhs);
        auto& rhsPlainValue = std::get<PlainValue>(rhs);
        return abs(rhsPlainValue.value - lhsPlainValue.value) < FLT_EPSILON;
    } else {
        return false;
    }
}

bool operator!=(const ComplexValue& lhs, const ComplexValue& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const ComplexValue& complexValue) {
    if (std::holds_alternative<PercentValue>(complexValue)) {
        auto percentValue = std::get<PercentValue>(complexValue);
        return os << percentValue.value << "%";
    } else if (std::holds_alternative<DpValue>(complexValue)) {
        auto dpValue = std::get<DpValue>(complexValue);
        return os << dpValue.value << "dp";
    } else {
        auto plainValue = std::get<PlainValue>(complexValue);
        return os << plainValue << "px";
    }
}
