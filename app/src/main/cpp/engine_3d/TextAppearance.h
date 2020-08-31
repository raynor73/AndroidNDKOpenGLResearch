//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef ENGINE_3D_TEXT_APPEARANCE_H
#define ENGINE_3D_TEXT_APPEARANCE_H


#include <utility>
#include <string>
#include "UnitsConverter.h"

class TextAppearance {

    ComplexValue m_textSize;
    std::string m_fontPath;

public:
    TextAppearance(
            ComplexValue textSize,
            std::string fontPath
    ) : m_textSize(std::move(textSize)),
        m_fontPath(std::move(fontPath)) {}

    const ComplexValue& textSize() const { return m_textSize; }
    const std::string& fontPath() const { return m_fontPath; }

    bool operator==(const TextAppearance& other) const;
};


#endif //ENGINE_3D_TEXT_APPEARANCE_H
