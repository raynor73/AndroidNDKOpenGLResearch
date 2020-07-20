//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef RENDERING_ENGINE_CHARACTER_H
#define RENDERING_ENGINE_CHARACTER_H


#include <string>
#include <utility>
#include <glm/vec2.hpp>

class Character {

    std::string m_textureName;
    glm::ivec2 m_size;
    glm::ivec2 m_bearing;
    unsigned int m_advance;

public:
    Character(
            std::string textureName,
            glm::ivec2 size,
            glm::ivec2 bearing,
            unsigned int advance
    ) : m_textureName(std::move(textureName)),
        m_size(std::move(size)),
        m_bearing(std::move(bearing)),
        m_advance(advance) {}

    const std::string& textureName() const { return m_textureName; }

    const glm::ivec2& size() const { return m_size; }

    const glm::ivec2& bearing() const { return m_bearing; }

    unsigned int advance() const { return m_advance; }
};


#endif //RENDERING_ENGINE_CHARACTER_H
