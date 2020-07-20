//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef ENGINE_3D_TEXT_APPEARANCE_H
#define ENGINE_3D_TEXT_APPEARANCE_H


#include <utility>
#include <string>

class TextAppearance {

    unsigned int m_textSize;
    std::string m_fontPath;

public:
    TextAppearance(
            unsigned int textSize,
            std::string fontPath
    ) : m_textSize(textSize),
        m_fontPath(std::move(fontPath)) {}

    unsigned int textSize() const { return m_textSize; }
    const std::string& fontPath() const { return m_fontPath; }

    bool operator==(const TextAppearance& other) const;
};


#endif //ENGINE_3D_TEXT_APPEARANCE_H
