#include <jni.h>
#include "glm/vec3.hpp"
#include "glm/geometric.hpp"
#include "main/GameWrapper.h"

static GameWrapper *gameWrapper = nullptr;
static float displayDensityFactor;

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperInit(
        JNIEnv *env,
        jobject that,
        jfloat display_density_factor
) {
    displayDensityFactor = display_density_factor;
}

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnDrawFrame(JNIEnv *env, jobject that) {
    gameWrapper->onDrawFrame();
}

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnSurfaceChanged(
        JNIEnv *env,
        jobject that,
        jint width,
        jint height
) {
    if (gameWrapper == nullptr) {
        gameWrapper = new GameWrapper(displayDensityFactor);
        glm::vec3 a;
        glm::length(a);
    }
    gameWrapper->onSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnSurfaceCreated(JNIEnv *env, jobject that) {
    gameWrapper->onSurfaceCreated();
}
