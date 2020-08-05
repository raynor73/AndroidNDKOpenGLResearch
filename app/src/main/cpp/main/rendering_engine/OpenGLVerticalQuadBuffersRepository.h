//
// Created by Igor Lapin on 21/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGL_VERTICAL_QUAD_BUFFERS_REPOSITORY_H
#define RENDERING_ENGINE_OPENGL_VERTICAL_QUAD_BUFFERS_REPOSITORY_H


#include <memory>
#include <string>
#include <vector>
#include <GLES2/gl2.h>
#include "OpenGLGeometryBuffersStorage.h"

struct VerticalQuadBuffers {
    GLuint vbo;
    IboInfo iboInfo;
};

class OpenGLVerticalQuadBuffersRepository {

    std::shared_ptr<OpenGLGeometryBuffersStorage> m_geometryBuffersStorage;

public:
    OpenGLVerticalQuadBuffersRepository(
            std::shared_ptr<OpenGLGeometryBuffersStorage> geometryBuffersStorage
    ) : m_geometryBuffersStorage(geometryBuffersStorage) {}
    OpenGLVerticalQuadBuffersRepository(const OpenGLVerticalQuadBuffersRepository&) = delete;
    OpenGLVerticalQuadBuffersRepository(OpenGLVerticalQuadBuffersRepository&&) = delete;

    VerticalQuadBuffers getBuffers();

    OpenGLVerticalQuadBuffersRepository& operator=(const OpenGLVerticalQuadBuffersRepository&) = delete;
    OpenGLVerticalQuadBuffersRepository& operator=(OpenGLVerticalQuadBuffersRepository&&) = delete;

private:
    static const std::string VERTICAL_QUAD_VERTEX_BUFFER_NAME;
    static const std::string VERTICAL_QUAD_INDEX_BUFFER_NAME;
    static const std::vector<float> QUAD_VERTEX_DATA;
    static const std::vector<uint16_t> QUAD_INDEX_DATA;

    VerticalQuadBuffers createBuffers();
};


#endif //RENDERING_ENGINE_OPENGL_VERTICAL_QUAD_BUFFERS_REPOSITORY_H
