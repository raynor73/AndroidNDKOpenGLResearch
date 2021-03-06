//
// Created by Igor Lapin on 02/06/2020.
//

#ifndef ENGINE_3D_GAMEOBJECTCOMPONENT_H
#define ENGINE_3D_GAMEOBJECTCOMPONENT_H


#include <memory>
#include <string>
#include <exception>
#include <sstream>
#include <main/L.h>

class GameObject;

class GameObjectComponent {

protected:
    bool m_isEnabled;
    GameObject* m_gameObject = nullptr;

    void throwErrorIfNoGameObject() const {
        if (m_gameObject == nullptr) {
            std::stringstream ss;
            ss << "No game object for component of type " << typeName();
            throw std::domain_error(ss.str());
        }
    }

public:
    GameObjectComponent() : m_isEnabled(true) {}
    GameObjectComponent(const GameObjectComponent&) = delete;
    GameObjectComponent(GameObjectComponent&&) = delete;
    virtual ~GameObjectComponent() = default;

    bool isEnabled() const { return m_isEnabled; }
    virtual void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    virtual const std::string& typeName() const = 0;

    GameObject* gameObject() { return m_gameObject; }
    void setGameObject(GameObject* gameObject) { m_gameObject = gameObject; }

    virtual void update() {};
    virtual void onAttachedToGameObject() {};
    virtual void onDetachedFromGameObject() {};
    virtual void onGameObjectAttachedToParent() {};
    virtual void onGameObjectDetachedFromParent() {};

    GameObjectComponent& operator=(const GameObjectComponent&) = delete;
    GameObjectComponent& operator=(GameObjectComponent&&) = delete;
    virtual std::shared_ptr<GameObjectComponent> clone() = 0;
};


#endif //ENGINE_3D_GAMEOBJECTCOMPONENT_H
