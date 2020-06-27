//
// Created by Igor Lapin on 02/06/2020.
//

#ifndef ENGINE_3D_GAMEOBJECTCOMPONENT_H
#define ENGINE_3D_GAMEOBJECTCOMPONENT_H


#include <memory>
#include <string>

class GameObject;

class GameObjectComponent {

protected:
    bool m_isEnabled;
    std::shared_ptr<GameObject> m_gameObject;

public:
    GameObjectComponent() : m_isEnabled(true) {}
    GameObjectComponent(const GameObjectComponent&) = delete;
    GameObjectComponent(GameObjectComponent&&) = delete;
    virtual ~GameObjectComponent() = default;

    bool isEnabled() const { return m_isEnabled; }
    void setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

    virtual std::string typeName() const = 0;

    std::shared_ptr<GameObject> gameObject() { return m_gameObject; }
    void setGameObject(const std::shared_ptr<GameObject>& gameObject) { m_gameObject = gameObject; }

    virtual void update() {};
    virtual void onAttachedToGameObject() {};
    virtual void onDetachedFromGameObject() {};

    GameObjectComponent& operator=(const GameObjectComponent&) = delete;
    GameObjectComponent& operator=(GameObjectComponent&&) = delete;
    virtual std::shared_ptr<GameObjectComponent> clone() = 0;
};


#endif //ENGINE_3D_GAMEOBJECTCOMPONENT_H
