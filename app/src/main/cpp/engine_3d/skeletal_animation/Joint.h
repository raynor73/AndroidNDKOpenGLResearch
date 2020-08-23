//
// Created by Igor Lapin on 22/08/2020.
//

#ifndef ENGINE_3D_JOINT_H
#define ENGINE_3D_JOINT_H


#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <glm/mat4x4.hpp>

class Joint {

    std::string m_name;
    int m_index;
    Joint* m_parent = nullptr;
    std::vector<std::shared_ptr<Joint>> m_children;
    glm::mat4 m_invertedBindTransform;

public:
    Joint(
            std::string name,
            int index,
            glm::mat4 invertedBindTransform
    ) : m_name(std::move(name)),
        m_index(index),
        m_invertedBindTransform(std::move(invertedBindTransform))
    {}

    void addChild(std::shared_ptr<Joint> joint) {
        joint->m_parent = this;
        m_children.emplace_back(joint);
    }

    const std::string& name() const { return m_name; }
    int index() const { return m_index; }
    Joint* parent() const { return m_parent; }
    const glm::mat4& invertedBindTransform() const { return m_invertedBindTransform; }
    const std::vector<std::shared_ptr<Joint>>& children() const { return m_children; }
};


#endif //ENGINE_3D_JOINT_H
