//
// Created by Igor Lapin on 02/06/2020.
//

#ifndef ENGINE_3D_GAMEOBJECTCOMPONENT_H
#define ENGINE_3D_GAMEOBJECTCOMPONENT_H


#include <memory>
#include <string>

class GameObject;

class GameObjectComponent {

    bool m_isEnabled;
    std::string m_name;
    std::shared_ptr<GameObject> m_gameObject;

public:
    explicit GameObjectComponent(std::string name) {};
    virtual ~GameObjectComponent() = default;

    bool isEnabled() const { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    std::string name() const { return m_name; }

    std::shared_ptr<GameObject> gameObject() { return m_gameObject; }
    void setGameObject(const std::shared_ptr<GameObject>& gameObject) { m_gameObject = gameObject; }

    virtual void update() { };
    virtual void onAttachedToGameObject() {};
    virtual void onDetachedFromGameObject() {};

    virtual std::shared_ptr<GameObjectComponent> clone() = 0;
};


#endif //ENGINE_3D_GAMEOBJECTCOMPONENT_H
