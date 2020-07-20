//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H
#define RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H


#include <unordered_map>
#include <game/CharactersRepository.h>

class OpenGLFreeTypeCharactersRepository : public CharactersRepository {

    std::unordered_map<char, Character> m_characterMap;

public:
    virtual const Character& getCharacter(const TextAppearance& textAppearance, char c) override;
};


#endif //RENDERING_ENGINE_OPENGL_FREE_TYPE_CHARACTERS_REPOSITORY_H
