//
// Created by Igor Lapin on 29/08/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_ANDROIDOPENALSOUNDSCENE_H
#define ANDROID_NDK_OPENGL_RESEARCH_ANDROIDOPENALSOUNDSCENE_H


#include <engine_3d/SoundStorage.h>
#include <engine_3d/SoundScene.h>
#include <unordered_map>
#include <set>
#include <AL/al.h>
#include <AL/alc.h>

class AndroidOpenALSoundScene : public SoundScene {

    static const int SAMPLE_RATE = 44100; // Hz

    ALCdevice* m_audioDevice = nullptr;
    ALCcontext* m_audioContext = nullptr;

    std::shared_ptr<SoundStorage> m_soundStorage;

    std::unordered_map<std::string, ALuint> m_soundNameToSoundBufferMap;
    std::unordered_map<std::string, ALuint> m_playerNameToSoundSourceMap;

    bool m_isScenePaused = false;
    std::set<ALuint> m_sourcesThatShouldBeResumed;

public:
    AndroidOpenALSoundScene(std::shared_ptr<SoundStorage> soundStorage);
    AndroidOpenALSoundScene(const AndroidOpenALSoundScene&) = delete;
    AndroidOpenALSoundScene(AndroidOpenALSoundScene&&) = delete;

    virtual ~AndroidOpenALSoundScene() override;

    virtual void update() override;

    virtual void pause() override;

    virtual void resume() override;

    virtual void clear() override;

    virtual void setSoundListenerTransform(
            const glm::vec3& position,
            const glm::vec3& lookAt,
            const glm::vec3& up
    ) override;

    virtual void setSoundListenerGain(float gain) override;

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
    ) override;

    virtual void removeSoundPlayer(const std::string& playerName) override;

    virtual void startSoundPlayer(const std::string& playerName, bool isLooped) override;

    virtual void pauseSoundPlayer(const std::string& playerName) override;

    virtual void resumeSoundPlayer(const std::string& playerName) override;

    virtual void stopSoundPlayer(const std::string& playerName) override;

    virtual void setSoundPlayerPosition(const std::string& playerName, const glm::vec3& position) override;

    virtual void setSoundPlayerGain(const std::string& playerName, float gain) override;

    virtual void setSoundPlayerPitch(const std::string& playerName, float pitch) override;

    AndroidOpenALSoundScene& operator=(const AndroidOpenALSoundScene&) = delete;
    AndroidOpenALSoundScene& operator=(AndroidOpenALSoundScene&&) = delete;

private:
    ALuint getSoundSource(const std::string& playerName);
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_ANDROIDOPENALSOUNDSCENE_H
