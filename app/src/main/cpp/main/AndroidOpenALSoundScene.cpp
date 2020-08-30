//
// Created by Igor Lapin on 29/08/2020.
//

#include <sstream>
#include <main/L.h>
#include <engine_3d/Constants.h>
#include "AndroidOpenALSoundScene.h"
#include "Constants.h"

AndroidOpenALSoundScene::AndroidOpenALSoundScene(
        std::shared_ptr<SoundStorage> soundStorage
) : m_soundStorage(std::move(soundStorage))
{
    m_audioDevice = alcOpenDevice(nullptr);
    if (m_audioDevice == nullptr) {
        throw std::domain_error("Failed to open OpenAL device");
    }

    m_audioContext = alcCreateContext(m_audioDevice, nullptr);
    if (m_audioContext == nullptr || alcMakeContextCurrent(m_audioContext) == ALC_FALSE) {
        /*if (ctx != NULL)
            alcDestroyContext(ctx);
        alcCloseDevice(device);*/
        throw std::domain_error("Failed to create or use OpenAL context");
    }

    alListener3f(AL_POSITION, 0, 0, 0);
    float orientationData[6];
    orientationData[0] = Engine3D::Constants::DEFAULT_FORWARD_DIRECTION.x;
    orientationData[1] = Engine3D::Constants::DEFAULT_FORWARD_DIRECTION.y;
    orientationData[2] = Engine3D::Constants::DEFAULT_FORWARD_DIRECTION.z;
    orientationData[3] = Engine3D::Constants::CAMERA_UP_DIRECTION.x;
    orientationData[4] = Engine3D::Constants::CAMERA_UP_DIRECTION.y;
    orientationData[5] = Engine3D::Constants::CAMERA_UP_DIRECTION.z;
    alListenerfv(AL_ORIENTATION, orientationData);
    alListener3f(AL_VELOCITY, 0, 0, 0);

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

AndroidOpenALSoundScene::~AndroidOpenALSoundScene() {
    /*ALCdevice *device;
    ALCcontext *ctx;

    ctx = alcGetCurrentContext();
    if(ctx == NULL)
        return;

    device = alcGetContextsDevice(ctx);*/

    AndroidOpenALSoundScene::clear();

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_audioContext);
    alcCloseDevice(m_audioDevice);
}

void AndroidOpenALSoundScene::update() {
    // Nothing required here
}

void AndroidOpenALSoundScene::pause() {
    if (m_isScenePaused) {
        L::e(App::Constants::LOG_TAG, "Sound scene already paused");
        return;
    }

    m_isScenePaused = true;

    for (auto& [playerName, soundSource] : m_playerNameToSoundSourceMap) {
        ALint sourceState;
        alGetSourcei(soundSource, AL_SOURCE_STATE, &sourceState);
        if (sourceState == AL_PLAYING) {
            m_sourcesThatShouldBeResumed.insert(soundSource);
            alSourcePause(soundSource);
        }
    }
}

void AndroidOpenALSoundScene::resume() {
    if (!m_isScenePaused) {
        L::e(App::Constants::LOG_TAG, "Sound scene is not paused");
        return;
    }

    m_isScenePaused = false;

    for (auto& soundSource : m_sourcesThatShouldBeResumed) {
        alSourcePlay(soundSource);
    }
}

void AndroidOpenALSoundScene::clear() {
    for (auto& [playerName, soundSource] : m_playerNameToSoundSourceMap) {
        alDeleteSources(1, &soundSource);
    }

    for (auto& [soundName, soundBuffer] : m_soundNameToSoundBufferMap) {
        alDeleteBuffers(1, &soundBuffer);
    }
}

void AndroidOpenALSoundScene::setSoundListenerTransform(
        const glm::vec3& position,
        const glm::vec3& lookAt,
        const glm::vec3& up
) {
    alListener3f(AL_POSITION, position.x, position.y, position.z);

    float orientationData[6];
    orientationData[0] = lookAt.x;
    orientationData[1] = lookAt.y;
    orientationData[2] = lookAt.z;
    orientationData[3] = up.x;
    orientationData[4] = up.y;
    orientationData[5] = up.z;
    alListenerfv(AL_ORIENTATION, orientationData);
}

void AndroidOpenALSoundScene::setSoundListenerGain(float gain) {
    alListenerf(AL_GAIN, gain);
}

void AndroidOpenALSoundScene::addSoundPlayer(
        const std::string& playerName,
        const std::string& soundName,
        const glm::vec3& position,
        float gain,
        float pitch,
        float rolloffFactor,
        float referenceDistance,
        float maxDistance,
        bool isRelativeToListener
) {
    if (m_playerNameToSoundSourceMap.count(playerName) > 0) {
        std::stringstream ss;
        ss << "Player " << playerName << " is already exist";
        throw std::domain_error(ss.str());
    }

    ALuint soundBuffer;
    if (m_soundNameToSoundBufferMap.count(soundName) > 0) {
        soundBuffer = m_soundNameToSoundBufferMap.at(soundName);
    } else {
        alGenBuffers(1, &soundBuffer);
        auto soundData = m_soundStorage->getSound(soundName);
        alBufferData(soundBuffer, AL_FORMAT_MONO16, soundData.data(), soundData.size(), SAMPLE_RATE);
        m_soundNameToSoundBufferMap.insert({ soundName, soundBuffer });
    }

    ALuint soundSource;
    alGenSources(1, &soundSource);
    alSource3i(soundSource, AL_POSITION, position.x, position.y, position.z);
    alSourcef(soundSource, AL_GAIN, gain);
    alSourcef(soundSource, AL_PITCH, pitch);
    alSourcei(soundSource, AL_BUFFER, soundBuffer);
    alSourcef(soundSource, AL_ROLLOFF_FACTOR, rolloffFactor);
    alSourcef(soundSource, AL_REFERENCE_DISTANCE, referenceDistance);
    alSourcef(soundSource, AL_MAX_DISTANCE, maxDistance);
    alSourcei(soundSource, AL_SOURCE_RELATIVE, isRelativeToListener ? AL_TRUE : AL_FALSE);

    m_playerNameToSoundSourceMap.insert({ playerName, soundSource });
}

void AndroidOpenALSoundScene::removeSoundPlayer(const std::string& playerName) {
    auto soundSource = getSoundSource(playerName);
    alDeleteSources(1, &soundSource);
    m_playerNameToSoundSourceMap.erase(playerName);
    m_sourcesThatShouldBeResumed.erase(soundSource);
}

void AndroidOpenALSoundScene::startSoundPlayer(const std::string& playerName, bool isLooped) {
    auto soundSource = getSoundSource(playerName);

    alSourcei(soundSource, AL_LOOPING, isLooped ? AL_TRUE : AL_FALSE);

    if (m_isScenePaused) {
        m_sourcesThatShouldBeResumed.insert(soundSource);
    } else {
        ALint sourceState;
        alGetSourcei(soundSource, AL_SOURCE_STATE, &sourceState);
        if (sourceState != AL_PLAYING) {
            alSourcePlay(soundSource);
        }
    }
}

void AndroidOpenALSoundScene::pauseSoundPlayer(const std::string& playerName) {
    auto soundSource = getSoundSource(playerName);

    if (m_isScenePaused) {
        m_sourcesThatShouldBeResumed.erase(soundSource);
    } else {
        ALint sourceState;
        alGetSourcei(soundSource, AL_SOURCE_STATE, &sourceState);
        if (sourceState == AL_PLAYING) {
            alSourcePause(soundSource);
        }
    }
}

void AndroidOpenALSoundScene::resumeSoundPlayer(const std::string& playerName) {
    auto soundSource = getSoundSource(playerName);

    if (m_isScenePaused) {
        m_sourcesThatShouldBeResumed.insert(soundSource);
    } else {
        ALint sourceState;
        alGetSourcei(soundSource, AL_SOURCE_STATE, &sourceState);
        if (sourceState == AL_PAUSED) {
            alSourcePlay(soundSource);
        }
    }
}

void AndroidOpenALSoundScene::stopSoundPlayer(const std::string& playerName) {
    auto soundSource = getSoundSource(playerName);
    alSourceStop(soundSource);
    m_sourcesThatShouldBeResumed.erase(soundSource);
}

void AndroidOpenALSoundScene::setSoundPlayerPosition(const std::string& playerName, const glm::vec3& position) {
    alSource3f(getSoundSource(playerName), AL_POSITION, position.x, position.y, position.z);
}

void AndroidOpenALSoundScene::setSoundPlayerGain(const std::string& playerName, float gain) {
    alSourcef(getSoundSource(playerName), AL_GAIN, gain);
}

void AndroidOpenALSoundScene::setSoundPlayerPitch(const std::string& playerName, float pitch) {
    alSourcef(getSoundSource(playerName), AL_PITCH, pitch);
}

ALuint AndroidOpenALSoundScene::getSoundSource(const std::string& playerName) {
    if (m_playerNameToSoundSourceMap.count(playerName) == 0) {
        std::stringstream ss;
        ss << "Player " << playerName << " not found";
        throw std::domain_error(ss.str());
    }

    return m_playerNameToSoundSourceMap.at(playerName);
}
