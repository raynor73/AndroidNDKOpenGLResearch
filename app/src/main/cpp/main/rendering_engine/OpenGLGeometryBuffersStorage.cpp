//
// Created by Igor Lapin on 06/07/2020.
//

#include <sstream>
#include <exception>
#include <utility>
#include "OpenGLGeometryBuffersStorage.h"

std::optional<GLuint> OpenGLGeometryBuffersStorage::findVbo(const std::string &name) {
    if (m_vbos.count(name) == 0) {
        return std::optional<GLuint>();
    } else {
        return m_vbos[name];
    }
}

std::optional<IboInfo> OpenGLGeometryBuffersStorage::findIbo(const std::string &name) {
    if (m_ibos.count(name) == 0) {
        return std::optional<IboInfo>();
    } else {
        return m_ibos[name];
    }
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
    if (m_vbos.count(name) > 0) {
        std::stringstream ss;
        ss << "VBO " << name << " already exists";
        throw std::domain_error(ss.str());
    }

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
        m_buffersCreationParams[name] = VertexBufferCreationParams { name, vertexData };
    }

    m_openGlErrorDetector->checkOpenGLErrors("createStaticVertexBuffer");

    return buffer;
}

IboInfo OpenGLGeometryBuffersStorage::createStaticIndexBuffer(
        const std::string& name,
        std::vector<uint16_t> indices,
        bool isBeingRestored
) {
    if (m_ibos.count(name) > 0) {
        std::stringstream ss;
        ss << "IBO " << name << " already exists";
        throw std::domain_error(ss.str());
    }

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
        m_buffersCreationParams[name] = IndexBufferCreationParams { name, indices };
    }

    m_openGlErrorDetector->checkOpenGLErrors("createStaticIndexBuffer");

    return iboInfo;
}

void OpenGLGeometryBuffersStorage::restoreBuffers() {
    m_vbos.clear();
    m_ibos.clear();

    for (auto& entry : m_buffersCreationParams) {
        if (std::holds_alternative<VertexBufferCreationParams>(entry.second)) {
            auto vertexBufferCreationParams = std::get<VertexBufferCreationParams>(entry.second);
            createStaticVertexBuffer(vertexBufferCreationParams.name, vertexBufferCreationParams.vertexData, true);
        } else if (std::holds_alternative<IndexBufferCreationParams>(entry.second)) {
            auto indexBufferCreationParams = std::get<IndexBufferCreationParams>(entry.second   );
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
    m_vbos.erase(name);

    m_buffersCreationParams.erase(name);

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
    m_ibos.erase(name);

    m_buffersCreationParams.erase(name);

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

std::vector<std::string> OpenGLGeometryBuffersStorage::retrieveStoredVboNames() const {
    auto names = std::vector<std::string>(m_vbos.size());

    for (auto& entry : m_vbos) {
        names.emplace_back(entry.first);
    }

    return names;
}

std::vector<std::string> OpenGLGeometryBuffersStorage::retrieveStoredIboNames() const {
    auto names = std::vector<std::string>(m_ibos.size());

    for (auto& entry : m_ibos) {
        names.emplace_back(entry.first);
    }

    return names;
}
