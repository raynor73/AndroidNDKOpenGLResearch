//
// Created by Igor Lapin on 03/07/2020.
//

#ifndef ENGINE_3D_VERTEX_H
#define ENGINE_3D_VERTEX_H


#include <glm/glm.hpp>

class Vertex {

    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_uv;

public:
    static constexpr int VERTEX_POSITION_COMPONENTS = 3;
    static constexpr int VERTEX_NORMAL_COMPONENTS = 3;
    static constexpr int VERTEX_UV_COMPONENTS = 2;
    static constexpr int VERTEX_COMPONENTS = VERTEX_POSITION_COMPONENTS + VERTEX_NORMAL_COMPONENTS + VERTEX_UV_COMPONENTS;

    Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv) :
        m_position(position),
        m_normal(normal),
        m_uv(uv)
    {}

    const glm::vec3& position() const { return m_position; }
    void setPosition(const glm::vec3& position) { m_position = position; }

    const glm::vec3& normal() const { return m_normal; }
    void setNormal(const glm::vec3& normal) { m_normal = normal; }

    const glm::vec2& uv() const { return m_uv; }
    void setUv(const glm::vec2& uv) { m_uv = uv; }
};


#endif //ENGINE_3D_VERTEX_H
