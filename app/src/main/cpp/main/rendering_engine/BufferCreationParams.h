//
// Created by Igor Lapin on 06/07/2020.
//

#ifndef RENDERING_ENGINE_BUFFERCREATIONPARAMS_H
#define RENDERING_ENGINE_BUFFERCREATIONPARAMS_H


#include <string>
#include <vector>

struct BufferCreationParams {
    std::string name;
};

struct IndexBufferCreationParams : public BufferCreationParams {
    std::vector<float> vertexData;
};

struct VertexBufferCreationParams : public BufferCreationParams {
    std::vector<uint16_t> indexData;
};

#endif //RENDERING_ENGINE_BUFFERCREATIONPARAMS_H
