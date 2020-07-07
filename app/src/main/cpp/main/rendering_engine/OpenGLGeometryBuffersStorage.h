//
// Created by Igor Lapin on 06/07/2020.
//

#ifndef RENDERING_ENGINE_OPENGLGEOMETRYBUFFERSSTORAGE_H
#define RENDERING_ENGINE_OPENGLGEOMETRYBUFFERSSTORAGE_H


#include <memory>
#include <utility>
#include <unordered_map>
#include <string>
#include <GLES2/gl2.h>
#include <vector>
#include <main/OpenGLErrorDetector.h>
#include <game/MeshStorage.h>
#include <variant>
#include "IboInfo.h"
#include "BufferCreationParams.h"

class OpenGLGeometryBuffersStorage {

    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;

    std::unordered_map<std::string, GLuint> m_vbos;
    std::unordered_map<std::string, IboInfo> m_ibos;

    std::vector<std::variant<VertexBufferCreationParams, IndexBufferCreationParams>> m_buffersCreationParams;

public:
    OpenGLGeometryBuffersStorage(std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector) :
        m_openGlErrorDetector(std::move(openGlErrorDetector)) {}
    OpenGLGeometryBuffersStorage(const OpenGLGeometryBuffersStorage&) = delete;
    OpenGLGeometryBuffersStorage(OpenGLGeometryBuffersStorage&&) = delete;

    GLuint createStaticVertexBuffer(std::string name, std::vector<float> vertexData);
    IboInfo createStaticIndexBuffer(std::string name, std::vector<uint16_t> indices);

    GLuint findVbo(const std::string& name);
    IboInfo findIbo(const std::string& name);

    void restoreBuffers();

    void removeStaticVertexBuffer(const std::string& name);
    void removeStaticIndexBuffer(const std::string& name);

    void removeAllBuffers();

    OpenGLGeometryBuffersStorage& operator=(const OpenGLGeometryBuffersStorage&) = delete;
    OpenGLGeometryBuffersStorage& operator=(const OpenGLGeometryBuffersStorage&&) = delete;
};


#endif //RENDERING_ENGINE_OPENGLGEOMETRYBUFFERSSTORAGE_H
