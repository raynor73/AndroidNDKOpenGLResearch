//
// Created by Igor Lapin on 10/07/2020.
//

#include "ShaderSourcePreprocessor.h"

std::string ShaderSourcePreprocessor::loadShaderSource(const std::string& path) {
    auto source = m_shaderSourceRepository->loadShaderSource(path);

    //source.fi

    return std::string();
}
