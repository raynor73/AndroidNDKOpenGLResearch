//
// Created by Igor Lapin on 31/08/2020.
//

#ifndef ENGINE_3D_MUSIC_PLAYER_H
#define ENGINE_3D_MUSIC_PLAYER_H


#include <string>

class MusicPlayer {
public:
    ~MusicPlayer() = default;

    virtual void play(const std::string& path) = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;
    virtual void stop() = 0;
    virtual void setGain(float gain) = 0;
    virtual bool isPlaybackCompleted() = 0;
};


#endif //ENGINE_3D_MUSIC_PLAYER_H
