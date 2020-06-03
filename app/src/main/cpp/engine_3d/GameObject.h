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
    explicit GameObject(const std::string &name);
    virtual ~GameObject();

    const std::string &name() const { return m_name; }

    bool isEnabled() const { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    std::shared_ptr<GameObject> parent() { return m_parent; }
    const std::unordered_map<std::string, std::shared_ptr<GameObject>> children() { return m_children; }

    void addChild(std::shared_ptr<GameObject> child);
    void removeChild(std::shared_ptr<GameObject> child);

    void addComponent(std::shared_ptr<GameObjectComponent> component);
    void removeComponent(std::shared_ptr<GameObjectComponent> component);

    void update();
    void onAttachedToParent();
    void onDetachedFromParent();

    std::shared_ptr<GameObjectComponent> findComponent(const std::string &name);

    std::shared_ptr<GameObject> clone();
    std::shared_ptr<GameObject> clone(const std::string &cloneName);

private:
    bool m_isEnabled;
    std::string m_name;

    std::shared_ptr<GameObject> m_parent;
    std::unordered_map<std::string, std::shared_ptr<GameObject>> m_children;
    std::unordered_map<std::string, std::shared_ptr<GameObjectComponent>> m_components;

    static int s_cloneNamePostfix;

    static int nextCloneNamePostfix();
};


#endif //ENGINE_3D_GAMEOBJECT_H
