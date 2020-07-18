#include <jni.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <main/Constants.h>
#include <main/L.h>
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
        JavaVM* javaVm;
        env->GetJavaVM(&javaVm);
        gameWrapper = new GameWrapper(
                displayDensityFactor,
                javaVm,
                reinterpret_cast<jclass>(env->NewGlobalRef(env->GetObjectClass(that))),
                env->NewGlobalRef(that)
        );

        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            L::e(App::Constants::LOG_TAG, "Could not init FreeType library");
        }
    }
    gameWrapper->onSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnSurfaceCreated(JNIEnv *env, jobject that) {
    gameWrapper->onSurfaceCreated();
}
