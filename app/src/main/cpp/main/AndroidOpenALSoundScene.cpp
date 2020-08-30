//
// Created by Igor Lapin on 29/08/2020.
//

#include <sstream>
#include "AndroidOpenALSoundScene.h"

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
    //alListener3f(AL_ORIENTATION, )
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

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(m_audioContext);
    alcCloseDevice(m_audioDevice);
}

void AndroidOpenALSoundScene::update() {

}

void AndroidOpenALSoundScene::pause() {

}

void AndroidOpenALSoundScene::resume() {

}

void AndroidOpenALSoundScene::clear() {

}

void AndroidOpenALSoundScene::setSoundListenerTransform(
        const glm::vec3& position,
        const glm::vec3& lookAt,
        const glm::vec3& up
) {

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
    alSourcef(soundSource, AL_GAIN, calculateFinalGain(gain));
    alSourcef(soundSource, AL_PITCH, pitch);
    alSourcei(soundSource, AL_BUFFER, soundBuffer);
    alSourcef(soundSource, AL_ROLLOFF_FACTOR, rolloffFactor);
    alSourcef(soundSource, AL_REFERENCE_DISTANCE, referenceDistance);
    alSourcef(soundSource, AL_MAX_DISTANCE, maxDistance);
    alSourcei(soundSource, AL_SOURCE_RELATIVE, isRelativeToListener ? AL_TRUE : AL_FALSE);

    m_playerNameToSoundSourceMap.insert({ playerName, soundSource });
}

void AndroidOpenALSoundScene::removeSoundPlayer(const std::string& playerName) {

}

void AndroidOpenALSoundScene::startSoundPlayer(const std::string& playerName, bool isLooped) {
    //alSourcei(soundSource, AL_LOOPING, AL_TRUE);
    //alSourcePlay(soundSource);
}

void AndroidOpenALSoundScene::pauseSoundPlayer(const std::string& playerName) {

}

void AndroidOpenALSoundScene::resumeSoundPlayer(const std::string& playerName) {

}

void AndroidOpenALSoundScene::stopSoundPlayer(const std::string& playerName) {

}

void AndroidOpenALSoundScene::setSoundPlayerPosition(const std::string& playerName, const glm::vec3& position) {

}

void AndroidOpenALSoundScene::setSoundPlayerGain(const std::string& playerName, float gain) {

}

void AndroidOpenALSoundScene::setSoundPlayerPitch(const std::string& playerName, float gain) {

}
