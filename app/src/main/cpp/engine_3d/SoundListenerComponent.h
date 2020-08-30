//
// Created by Igor Lapin on 30/08/2020.
//

#ifndef ENGINE_3D_SOUND_LISTENER_COMPONENT_H
#define ENGINE_3D_SOUND_LISTENER_COMPONENT_H


#include <memory>
#include "SoundScene.h"
#include "GameObjectComponent.h"

class SoundListenerComponent : public GameObjectComponent {

    std::shared_ptr<SoundScene> m_soundScene;

public:
    SoundListenerComponent(std::shared_ptr<SoundScene> soundScene);

    static const std::string TYPE_NAME;

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    virtual void update() override;
};


#endif //ENGINE_3D_SOUND_LISTENER_COMPONENT_H
