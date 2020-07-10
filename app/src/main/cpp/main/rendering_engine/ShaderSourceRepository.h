//
// Created by Igor Lapin on 10/07/2020.
//

#ifndef RENDERING_ENGINE_SHADERSOURCEREPOSITORY_H
#define RENDERING_ENGINE_SHADERSOURCEREPOSITORY_H


#include <string>

class ShaderSourceRepository {

public:
    virtual ~ShaderSourceRepository() = default;

    virtual std::string loadShaderSource(const std::string& path) = 0;
};


#endif //RENDERING_ENGINE_SHADERSOURCEREPOSITORY_H
