//
// Created by Igor Lapin on 23/07/2020.
//

#ifndef GAME_TEXTURES_REPOSITORY_H
#define GAME_TEXTURES_REPOSITORY_H


#include <string>
#include <vector>

class TexturesRepository {

public:
    virtual void createTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data
    ) = 0;

    virtual void createDisplayDensityFactorAwareTexture(const std::string& name, const std::string& path) = 0;

    /*virtual void createGlyphTexture(
            const std::string& name,
            uint_t width,
            uint_t height,
            const std::vector<uint8_t>& data
    ) = 0;*/
};

#endif //GAME_TEXTURES_REPOSITORY_H
