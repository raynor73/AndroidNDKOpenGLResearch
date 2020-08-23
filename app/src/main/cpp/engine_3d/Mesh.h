//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef ENGINE_3D_MESH_H
#define ENGINE_3D_MESH_H


#include <vector>
#include "Vertex.h"

class Mesh {

    std::vector<Vertex> m_vertices;
    std::vector<uint16_t> m_indices;

public:
    Mesh() = default;
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices) :
        m_vertices(vertices),
        m_indices(indices)
    {}

    const std::vector<Vertex>& vertices() const { return m_vertices; }
    const std::vector<uint16_t>& indices() const { return m_indices; }

    void setJointIndices(size_t vertexIndex, const glm::ivec3& indices);
    void setJointWeights(size_t vertexIndex, const glm::vec3 weights);
};


#endif //ENGINE_3D_MESH_H
