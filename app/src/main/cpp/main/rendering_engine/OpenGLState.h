//
// Created by Igor Lapin on 28/06/2020.
//

#ifndef RENDERING_ENGINE_OPENGLSTATE_H
#define RENDERING_ENGINE_OPENGLSTATE_H


#include <GLES2/gl2.h>

struct BlendFunction {
    GLenum sFactor;
    GLenum dFactor;
};

struct Viewport {
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

struct Scissor {
    GLint x;
    GLint y;
    GLsizei width;
    GLsizei height;
};

struct OpenGLState {
    Viewport viewport;
    Scissor scissor;
    bool blend;
    BlendFunction blendFunction;
    bool depthMask;
    GLenum depthFunction;
};


#endif //RENDERING_ENGINE_OPENGLSTATE_H
