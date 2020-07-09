//
// Created by Igor Lapin on 06/07/2020.
//

#include <sstream>
#include <exception>
#include <utility>
#include "OpenGLGeometryBuffersStorage.h"

GLuint OpenGLGeometryBuffersStorage::getVbo(const std::string &name) {
    if (m_vbos.count(name) == 0) {
        std::stringstream ss;
        ss << "No vertex buffer " << name << " to remove";
        throw std::domain_error(ss.str());
    }

    return m_vbos[name];
}

IboInfo OpenGLGeometryBuffersStorage::getIbo(const std::string &name) {
    if (m_ibos.count(name) == 0) {
        std::stringstream ss;
        ss << "Static index buffer " << name << " not found";
        throw std::domain_error(ss.str());
    }

    return m_ibos[name];
}

GLuint OpenGLGeometryBuffersStorage::createStaticVertexBuffer(
        const std::string& name,
        const std::vector<float>& vertexData
) {
    return createStaticVertexBuffer(name, vertexData, false);
}

IboInfo OpenGLGeometryBuffersStorage::createStaticIndexBuffer(
        const std::string &name,
        const std::vector<uint16_t> &indices
) {
    return createStaticIndexBuffer(name, indices, false);
}

GLuint OpenGLGeometryBuffersStorage::createStaticVertexBuffer(
        const std::string& name,
        std::vector<float> vertexData,
        bool isBeingRestored
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    glBufferData(
            GL_ARRAY_BUFFER,
            vertexData.size() * sizeof(float),
            vertexData.data(),
            GL_STATIC_DRAW
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_vbos[name] = buffer;
    if (!isBeingRestored) {
        m_buffersCreationParams.emplace_back(VertexBufferCreationParams { name, vertexData });
    }

    m_openGlErrorDetector->checkOpenGLErrors("createStaticVertexBuffer");

    return buffer;
}

IboInfo OpenGLGeometryBuffersStorage::createStaticIndexBuffer(
        const std::string& name,
        std::vector<uint16_t> indices,
        bool isBeingRestored
) {
    GLuint buffer;
    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(uint16_t),
            indices.data(),
            GL_STATIC_DRAW
    );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    auto iboInfo = IboInfo { buffer, indices.size() };
    m_ibos[name] = iboInfo;
    if (!isBeingRestored) {
        m_buffersCreationParams.emplace_back(IndexBufferCreationParams { name, indices });
    }

    m_openGlErrorDetector->checkOpenGLErrors("createStaticIndexBuffer");

    return iboInfo;
}

void OpenGLGeometryBuffersStorage::restoreBuffers() {
    m_vbos.clear();
    m_ibos.clear();

    for (auto& bufferCreationParams : m_buffersCreationParams) {
        if (std::holds_alternative<VertexBufferCreationParams>(bufferCreationParams)) {
            auto vertexBufferCreationParams = std::get<VertexBufferCreationParams>(bufferCreationParams);
            createStaticVertexBuffer(vertexBufferCreationParams.name, vertexBufferCreationParams.vertexData, true);
        } else if (std::holds_alternative<IndexBufferCreationParams>(bufferCreationParams)) {
            auto indexBufferCreationParams = std::get<IndexBufferCreationParams>(bufferCreationParams);
            createStaticIndexBuffer(indexBufferCreationParams.name, indexBufferCreationParams.indices, true);
        }
    }
}

void OpenGLGeometryBuffersStorage::removeStaticVertexBuffer(const std::string &name) {
    if (m_vbos.count(name) == 0) {
        std::stringstream ss;
        ss << "No vertex buffer " << name << " to remove";
        throw std::domain_error(ss.str());
    }

    auto buffer = m_vbos[name];
    glDeleteBuffers(1, &buffer);

    m_openGlErrorDetector->checkOpenGLErrors("removeStaticVertexBuffer");
}

void OpenGLGeometryBuffersStorage::removeStaticIndexBuffer(const std::string &name) {
    if (m_ibos.count(name) == 0) {
        std::stringstream ss;
        ss << "No index buffer " << name << " to remove";
        throw std::domain_error(ss.str());
    }

    auto bufferInfo = m_ibos[name];
    glDeleteBuffers(1, &bufferInfo.ibo);

    m_openGlErrorDetector->checkOpenGLErrors("removeStaticIndexBuffer");
}

void OpenGLGeometryBuffersStorage::removeAllBuffers() {
    std::vector<std::string> namesToDelete;

    for (auto& entry : m_vbos) {
        namesToDelete.push_back(entry.first);
    }
    for (auto& name : namesToDelete) {
        removeStaticVertexBuffer(name);
    }

    namesToDelete.clear();
    for (auto& entry : m_ibos) {
        namesToDelete.push_back(entry.first);
    }
    for (auto& name : namesToDelete) {
        removeStaticIndexBuffer(name);
    }
}
