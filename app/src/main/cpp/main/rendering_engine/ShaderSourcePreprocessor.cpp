//
// Created by Igor Lapin on 10/07/2020.
//

#include "ShaderSourcePreprocessor.h"

const std::string ShaderSourcePreprocessor::INCLUDE_TOKEN_PREFIX = "#include \"";
const std::string ShaderSourcePreprocessor::INCLUDE_TOKEN_POSTFIX = "\"";

std::string ShaderSourcePreprocessor::loadShaderSource(const std::string& path) {
    auto source = m_shaderSourceRepository->loadShaderSource(path);

    for (size_t currentPosition = 0;;) {
        auto prefixPosition = source.find(INCLUDE_TOKEN_PREFIX, currentPosition);
        if (prefixPosition != std::string::npos) {
            auto postfixPosition = source.find(INCLUDE_TOKEN_POSTFIX, prefixPosition + INCLUDE_TOKEN_PREFIX.size());
            if (postfixPosition != std::string::npos) {
                auto includePath = source.substr(
                        prefixPosition + INCLUDE_TOKEN_PREFIX.size(),
                        postfixPosition - prefixPosition - INCLUDE_TOKEN_PREFIX.size() - INCLUDE_TOKEN_POSTFIX.size()
                );
                auto includeSource = m_shaderSourceRepository->loadShaderSource(includePath);
                source.replace(
                        prefixPosition + INCLUDE_TOKEN_POSTFIX.size(),
                        includePath.size() + INCLUDE_TOKEN_POSTFIX.size(),
                        includeSource
                );
                currentPosition = prefixPosition + includeSource.size();
                continue;
            }
        }
        break;
    }

    return source;
}
