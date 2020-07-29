#include <jni.h>
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
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperPutTouchEventIntoQueue(
        JNIEnv* env,
        jobject that,
        jobject touch_event
) {
    jclass touchEventClass = env->GetObjectClass(touch_event);

    jfieldID idFieldID = env->GetFieldID(touchEventClass, "id", "I");
    jfieldID actionFieldID = env->GetFieldID(touchEventClass, "action", "I");
    jfieldID xFieldID = env->GetFieldID(touchEventClass, "x", "I");
    jfieldID yFieldID = env->GetFieldID(touchEventClass, "y", "I");

    jint id = env->GetIntField(touch_event, idFieldID);

    jint actionOrdinalValue = env->GetIntField(touch_event, actionFieldID);
    TouchEventType touchEventType;
    switch (actionOrdinalValue) {
        case 0:
            touchEventType = TouchEventType::DOWN;
            break;

        case 1:
            touchEventType = TouchEventType::MOVE;
            break;

        case 2:
            touchEventType = TouchEventType::UP;
            break;

        case 3:
            touchEventType = TouchEventType::CANCEL;
            break;

        default: {
            std::stringstream ss;
            ss << "Unexpected Action Ordinal value: " << actionOrdinalValue;
            throw std::domain_error(ss.str());
        }
    }

    jint x = env->GetIntField(touch_event, xFieldID);
    jint y = env->GetIntField(touch_event, yFieldID);

    gameWrapper->putTouchEventIntoQueue(std::make_shared<TouchEvent>(id, float(x), float(y), touchEventType));
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

        gameWrapper->requestSceneLoadAndStart(SceneType::SCENES_SELECTION_SCENE);
    }
    gameWrapper->onSurfaceChanged(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_ilapin_opengl_1research_GLSurfaceViewRenderer_gameWrapperOnSurfaceCreated(JNIEnv *env, jobject that) {
    gameWrapper->onSurfaceCreated();
}
