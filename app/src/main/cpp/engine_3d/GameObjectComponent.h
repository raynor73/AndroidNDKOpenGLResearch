//
// Created by Igor Lapin on 02/06/2020.
//

#ifndef ENGINE_3D_GAMEOBJECTCOMPONENT_H
#define ENGINE_3D_GAMEOBJECTCOMPONENT_H

#include <memory>

class GameObject;

class GameObjectComponent {

public:

private:
    bool m_isEnabled;
    std::weak_ptr<GameObject> m_gameObject;
};


#endif //ENGINE_3D_GAMEOBJECTCOMPONENT_H
