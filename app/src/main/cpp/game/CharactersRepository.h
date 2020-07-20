//
// Created by Igor Lapin on 20/07/2020.
//

#ifndef GAME_CHARACTERS_REPOSITORY_H
#define GAME_CHARACTERS_REPOSITORY_H


#include <engine_3d/Character.h>
#include <engine_3d/TextAppearance.h>

class CharactersRepository {

public:
    virtual const Character& getCharacter(const TextAppearance& textAppearance, char c) = 0;
};


#endif //GAME_CHARACTERS_REPOSITORY_H
