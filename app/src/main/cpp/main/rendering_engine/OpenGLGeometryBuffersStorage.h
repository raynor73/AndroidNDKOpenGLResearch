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
#include "IboInfo.h"

class OpenGLGeometryBuffersStorage {

    std::shared_ptr<OpenGLErrorDetector> m_openGlErrorDetector;
    std::shared_ptr<MeshStorage> m_meshStorage;

    std::unordered_map<std::string, GLuint> m_vbos;
    std::unordered_map<std::string, IboInfo> m_ibos;

public:
    OpenGLGeometryBuffersStorage(std::shared_ptr<OpenGLErrorDetector> openGlErrorDetector) :
        m_openGlErrorDetector(std::move(openGlErrorDetector)) {}
    OpenGLGeometryBuffersStorage(const OpenGLGeometryBuffersStorage&) = delete;
    OpenGLGeometryBuffersStorage(OpenGLGeometryBuffersStorage&&) = delete;

    void restoreBuffers();
    void createStaticVertexBuffer(const std::string& name, const std::vector<uint16_t> indices);

    OpenGLGeometryBuffersStorage& operator=(const OpenGLGeometryBuffersStorage&) = delete;
    OpenGLGeometryBuffersStorage& operator=(const OpenGLGeometryBuffersStorage&&) = delete;
};


#endif //RENDERING_ENGINE_OPENGLGEOMETRYBUFFERSSTORAGE_H
