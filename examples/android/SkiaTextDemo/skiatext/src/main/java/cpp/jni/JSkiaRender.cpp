#include "skiatext.h"
#include "GPUSurface.h"
#include <android/native_window_jni.h>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"skiatext",__VA_ARGS__)

namespace skiatext {
    static jfieldID Renderer_nativeContext;
}

using namespace skiatext;

extern "C" {
JNIEXPORT void setRenderer(JNIEnv *env, jobject thiz, Renderer *renderer) {
    auto old = reinterpret_cast<Renderer*>(env->GetLongField(thiz, Renderer_nativeContext));
    if (old != nullptr) {
        delete old;
    }
    env->SetLongField(thiz, Renderer_nativeContext, (jlong) renderer);
}

JNIEXPORT Renderer* getRenderer(JNIEnv *env, jobject thiz) {
    return reinterpret_cast<Renderer*>(env->GetLongField(thiz, Renderer_nativeContext));
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaRender_nativeInit(JNIEnv *env, jclass type) {
    jclass clazz = env->FindClass("com/lvpengwei/skiatext/SkiaRender");
    Renderer_nativeContext = env->GetFieldID(clazz, "nativeContext", "J");
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaRender_nativeSetup(JNIEnv* env, jobject thiz) {
    auto renderer = new Renderer();
    setRenderer(env, thiz, renderer);
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaRender_nativeRelease(JNIEnv* env, jobject thiz) {
    setRenderer(env, thiz, nullptr);
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaRender_nativeSetSurface(JNIEnv* env, jobject thiz,
                                                            jlong surfaceObject) {
    auto renderer = getRenderer(env, thiz);
    if (renderer == nullptr) {
        return;
    }
    auto surface = reinterpret_cast<Surface*>(surfaceObject);
    renderer->setSurface(surface);
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaRender_draw(JNIEnv* env, jobject thiz) {
    auto renderer = getRenderer(env, thiz);
    if (renderer == nullptr) {
        return;
    }
    renderer->draw();
}

JNIEXPORT jboolean Java_com_lvpengwei_skiatext_SkiaRender_flush(JNIEnv* env, jobject thiz) {
    auto renderer = getRenderer(env, thiz);
    if (renderer == nullptr) {
        return 0;
    }
    auto changed = renderer->flush();
    return static_cast<jboolean>(changed);
}

}
