//
// Created by Igor Lapin on 03/07/2020.
//

#include <utility>
#include <sstream>
#include <exception>
#include "MeshStorage.h"

void MeshStorage::putMesh(const std::string &name, Mesh mesh) {
    if (m_storage.count(name) > 0) {
        std::stringstream ss;
        ss << "Mesh " << name << " already stored";
        throw std::domain_error(ss.str());
    }

    m_storage[name] = std::move(mesh);
}

Mesh MeshStorage::findMesh(const std::string &name) {
    if (m_storage.count(name) == 0) {
        std::stringstream ss;
        ss << "Mesh " << name << " not found";
        throw std::domain_error(ss.str());
    }

    return m_storage[name];
}

void MeshStorage::removeMesh(const std::string &name) {
    if (m_storage.count(name) == 0) {
        std::stringstream ss;
        ss << "No mesh " << name << " to remove";
        throw std::domain_error(ss.str());
    }

    m_storage.erase(name);
}

void MeshStorage::removeAllMeshes() {
    m_storage.clear();
}
