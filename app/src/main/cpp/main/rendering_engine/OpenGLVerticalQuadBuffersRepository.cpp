//
// Created by Igor Lapin on 21/07/2020.
//

#include "OpenGLVerticalQuadBuffersRepository.h"

const std::string OpenGLVerticalQuadBuffersRepository::VERTICAL_QUAD_BUFFERS_NAME = "VERTICAL_QUAD_BUFFERS_NAME";

const std::vector<float> OpenGLVerticalQuadBuffersRepository::QUAD_VERTEX_DATA = {
        -0.5, -0.5, 0,
           0,    0, 1,
           0,    0,

        -0.5,  0.5, 0,
           0,    0, 1,
           0,    1,

         0.5,  0.5, 0,
           0,    0, 1,
           1,    1,

         0.5, -0.5, 0,
           0,    0, 1,
           1,    0
};
const std::vector<uint16_t> OpenGLVerticalQuadBuffersRepository::QUAD_INDEX_DATA = {
    0, 3, 2, 2, 1, 0
};

VerticalQuadBuffers OpenGLVerticalQuadBuffersRepository::getBuffers() {
    auto vboOptional = m_geometryBuffersStorage->findVbo(VERTICAL_QUAD_BUFFERS_NAME);
    auto iboInfoOptional = m_geometryBuffersStorage->findIbo(VERTICAL_QUAD_BUFFERS_NAME);

    if (!vboOptional || !iboInfoOptional) {
        return createBuffers();
    } else {
        return VerticalQuadBuffers { vboOptional.value(), iboInfoOptional.value() };
    }
}

VerticalQuadBuffers OpenGLVerticalQuadBuffersRepository::createBuffers() {
    auto vbo = m_geometryBuffersStorage->createStaticVertexBuffer(VERTICAL_QUAD_BUFFERS_NAME, QUAD_VERTEX_DATA);
    auto iboInfo = m_geometryBuffersStorage->createStaticIndexBuffer(VERTICAL_QUAD_BUFFERS_NAME, QUAD_INDEX_DATA);
    return VerticalQuadBuffers { vbo, iboInfo };
}
