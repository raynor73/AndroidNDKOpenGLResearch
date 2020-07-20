//
// Created by Igor Lapin on 20/07/2020.
//

#include "TextAppearance.h"

bool TextAppearance::operator==(const TextAppearance& other) const {
    if (this == &other) {
        return true;
    }

    return m_fontPath == other.m_fontPath && m_textSize == other.m_textSize;
}
