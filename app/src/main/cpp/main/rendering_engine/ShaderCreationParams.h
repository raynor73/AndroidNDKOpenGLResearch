//
// Created by Igor Lapin on 09/07/2020.
//

#ifndef RENDERING_ENGINE_SHADER_CREATION_PARAMS_H
#define RENDERING_ENGINE_SHADER_CREATION_PARAMS_H


#include <string>

struct ShaderCreationParams {
    std::string name;
    std::string source;
};

struct VertexShaderCreationParams : ShaderCreationParams {};

struct FragmentShaderCreationParams : ShaderCreationParams {};

struct ShaderProgramCreationParams {
    std::string name;
    std::string vertexShaderName;
    std::string fragmentShaderName;
};


#endif //RENDERING_ENGINE_SHADER_CREATION_PARAMS_H
