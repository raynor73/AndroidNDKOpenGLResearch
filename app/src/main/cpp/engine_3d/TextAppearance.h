//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef ENGINE_3D_TEXT_APPEARANCE_H
#define ENGINE_3D_TEXT_APPEARANCE_H


#include <utility>
#include <string>

class TextAppearance {

    unsigned int m_textSize;
    std::string m_fontName;

public:
    TextAppearance(
            unsigned int textSize,
            std::string fontName
    ) : m_textSize(textSize),
        m_fontName(std::move(fontName)) {}

    unsigned int textSize() const { return m_textSize; }
    const std::string& fontName() const { return m_fontName; }

    bool operator==(const TextAppearance& other) const;
};


#endif //ENGINE_3D_TEXT_APPEARANCE_H
