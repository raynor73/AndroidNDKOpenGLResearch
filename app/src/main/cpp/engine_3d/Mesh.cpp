//
// Created by Igor Lapin on 23/08/2020.
//

#include "Mesh.h"

void Mesh::setJointIndices(size_t vertexIndex, const glm::ivec3& indices) {
    m_vertices[vertexIndex].setJointIndices(indices);
}

void Mesh::setJointWeights(size_t vertexIndex, const glm::vec3 weights) {
    m_vertices[vertexIndex].setJointWeights(weights);
}
