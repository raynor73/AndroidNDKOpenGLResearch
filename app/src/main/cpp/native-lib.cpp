#include <jni.h>
#include <string>
#include "main/GameWrapper.h"

/*extern "C" JNIEXPORT jstring JNICALL
Java_ilapin_opengl_1research_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject that) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}*/

static GameWrapper gameWrapper;

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnDrawFrame(JNIEnv *env, jobject that) {
    gameWrapper.onDrawFrame();
}

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnSurfaceChanged(
        JNIEnv *env,
        jobject that,
        jint width,
        jint height
) {
    gameWrapper.onSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnSurfaceCreated(JNIEnv *env, jobject that) {
    gameWrapper.onSurfaceCreated();
}