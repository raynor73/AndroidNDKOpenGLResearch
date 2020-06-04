//
// Created by Igor Lapin on 03/06/2020.
//

#ifndef ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
#define ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H


class GameWrapper {

public:
    void onDrawFrame();
    void onSurfaceChanged(int width, int height);
    void onSurfaceCreated();
};


#endif //ANDROID_NDK_OPENGL_RESEARCH_GAMEWRAPPER_H
