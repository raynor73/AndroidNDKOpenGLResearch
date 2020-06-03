//
// Created by Igor Lapin on 02/06/2020.
//

#ifndef ENGINE_3D_GAMEOBJECT_H
#define ENGINE_3D_GAMEOBJECT_H


#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include "GameObjectComponent.h"

class GameObject {

public:
    GameObject(const std::string& name);
    virtual ~GameObject();

    const std::string& name() const { return m_name; }

    bool isEnabled() const { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    std::shared_ptr<GameObject> parent() { return m_parent.lock(); }
    const std::unordered_map<std::string, std::shared_ptr<GameObject>> children() { return m_children; }

    void addChild(GameObject& child) {
        /*if (_children.contains(child)) {
            error("Trying to add ${child.name} game object second time")
        }
        _children += child
        child._parent = this*/
        if (m_children.count(child.name())) {
            throw std::domain_error("Trying to add ${child.name} game object second time");
        }
    }

private:
    bool m_isEnabled;
    std::string m_name;

    std::weak_ptr<GameObject> m_parent;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_children;
    std::unordered_map<std::string, std::shared_ptr<GameObjectComponent>> m_components;

    std::string foo() {
        std::stringstream ss;
        ss << "123" << 42;
        return ss.str();
    }
};


#endif //ENGINE_3D_GAMEOBJECT_H
