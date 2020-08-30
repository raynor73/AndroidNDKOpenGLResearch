//
// Created by Igor Lapin on 30/08/2020.
//

#ifndef ENGINE_3D_SOUND_PLAYER_COMPONENT_H
#define ENGINE_3D_SOUND_PLAYER_COMPONENT_H


#include <string>
#include <memory>
#include <utility>
#include "GameObjectComponent.h"
#include "SoundScene.h"

class SoundPlayerComponent : public GameObjectComponent {

    std::string m_playerName;

    std::shared_ptr<SoundScene> m_soundScene;
    std::string m_soundName;
    float m_gain;
    float m_pitch;
    float m_rolloffFactor;
    float m_referenceDistance;
    float m_maxDistance;
    bool m_isLooped;
    bool m_isRelativeToListener;

    bool m_isSoundPlayerAddedToScene = false;
    bool m_isPlaying = false;

public:
    static const std::string TYPE_NAME;

    SoundPlayerComponent(
            std::shared_ptr<SoundScene> soundScene,
            std::string soundName,
            float gain,
            float pitch,
            float rolloffFactor,
            float referenceDistance,
            float maxDistance,
            bool isLooped,
            bool isRelativeToListener
    );

    virtual const std::string& typeName() const override { return TYPE_NAME; }

    virtual void onAttachedToGameObject() override;

    virtual void onDetachedFromGameObject() override;

    virtual std::shared_ptr<GameObjectComponent> clone() override;

    virtual void setEnabled(bool isEnabled) override;

    virtual void update() override;

    void play();
    void pause();
    void resume();
    void stop();

    float gain() const { return m_gain; }
    void setGain(float gain) { m_gain = gain; }

    float pitch() const { return m_pitch; }
    void setPitch(float pitch) { m_pitch = pitch; }

    bool isLooped() const { return m_isLooped; }
    void setIsLooped(bool isLooped) { m_isLooped = isLooped; }
};


#endif //ENGINE_3D_SOUND_PLAYER_COMPONENT_H
