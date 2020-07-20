//
// Created by Igor Lapin on 20/07/2020.
//

#include <exception>
#include <sstream>
#include "OpenGLFreeTypeCharactersRepository.h"

const Character& OpenGLFreeTypeCharactersRepository::getCharacter(const TextAppearance& textAppearance, char c) {
    if (m_characterMap.count(c) == 0) {
        std::stringstream ss;
        ss << "Character " << c << " not found";
    }

    return <#initializer#>;
}
