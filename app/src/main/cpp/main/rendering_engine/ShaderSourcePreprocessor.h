//
// Created by Igor Lapin on 10/07/2020.
//

#ifndef RENDERING_ENGINE_SHADERSOURCEPREPROCESSOR_H
#define RENDERING_ENGINE_SHADERSOURCEPREPROCESSOR_H


#include <memory>
#include <string>
#include "ShaderSourceRepository.h"

class ShaderSourcePreprocessor {

    std::shared_ptr<ShaderSourceRepository> m_shaderSourceRepository;

public:
    ShaderSourcePreprocessor(std::shared_ptr<ShaderSourceRepository> shaderSourceRepository) :
        m_shaderSourceRepository(shaderSourceRepository) {}
    ShaderSourcePreprocessor(const ShaderSourcePreprocessor&) = delete;

    std::string loadShaderSource(const std::string& path);

    ShaderSourcePreprocessor& operator=(const ShaderSourcePreprocessor&) = delete;
    ShaderSourcePreprocessor& operator=(ShaderSourcePreprocessor&&) = delete;

private:
    static const std::string INCLUDE_TOKEN_PREFIX;
    static const std::string INCLUDE_TOKEN_POSTFIX;
};


#endif //RENDERING_ENGINE_SHADERSOURCEPREPROCESSOR_H
