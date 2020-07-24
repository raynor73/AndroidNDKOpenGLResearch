//
// Created by Igor Lapin on 24/07/2020.
//

#ifndef RENDERING_ENGINE_TEXTURE_CREATION_PARAMS_H
#define RENDERING_ENGINE_TEXTURE_CREATION_PARAMS_H


#include <GLES2/gl2.h>
#include <string>
#include <vector>

struct TextureCreationParams {
    std::string name;
};

struct TextureFromMemoryCreationParams : public TextureCreationParams {
    uint_t width;
    uint_t height;
    std::vector<uint8_t> data;
};


#endif //RENDERING_ENGINE_TEXTURE_CREATION_PARAMS_H
