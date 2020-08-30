//
// Created by Igor Lapin on 29/08/2020.
//

#ifndef ENGINE_3D_SOUND_SCENE_H
#define ENGINE_3D_SOUND_SCENE_H


#include <string>
#include <glm/vec3.hpp>
#include <glm/ext.hpp>

class SoundScene {

public:
    virtual ~SoundScene() = default;

    virtual void update() = 0;

    virtual void pause() = 0;
    virtual void resume() = 0;

    virtual void clear() = 0;

    virtual void setSoundListenerTransform(const glm::vec3& position, const glm::vec3& lookAt, const glm::vec3& up) = 0;
    virtual void setSoundListenerGain(float gain) = 0;

    virtual void addSoundPlayer(
            const std::string& playerName,
            const std::string& soundName,
            const glm::vec3& position,
            float gain,
            float pitch,
            float rolloffFactor,
            float referenceDistance,
            float maxDistance,
            bool isRelativeToListener
    ) = 0;
    virtual void removeSoundPlayer(const std::string& playerName) = 0;

    virtual void startSoundPlayer(const std::string& playerName, bool isLooped) = 0;
    virtual void pauseSoundPlayer(const std::string& playerName) = 0;
    virtual void resumeSoundPlayer(const std::string& playerName) = 0;
    virtual void stopSoundPlayer(const std::string& playerName) = 0;

    virtual void setSoundPlayerPosition(const std::string& playerName, const glm::vec3& position) = 0;
    virtual void setSoundPlayerGain(const std::string& playerName, float gain) = 0;
    virtual void setSoundPlayerPitch(const std::string& playerName, float pitch) = 0;
};


#endif //ENGINE_3D_SOUND_SCENE_H
