//
// Created by lvpengwei on 2019/5/19.
//
#include "GPUSurface.h"
#include <android/native_window_jni.h>
#include <android/log.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"skiatext",__VA_ARGS__)

namespace skiatext {
    static jfieldID GPUSurface_nativeSurface;
}

using namespace skiatext;

extern "C" {

JNIEXPORT void setGPUSurface(JNIEnv* env, jobject thiz, GPUSurface* surface) {
    auto old = reinterpret_cast<GPUSurface*>(env->GetLongField(thiz, GPUSurface_nativeSurface));
    if (old != nullptr) {
        delete old;
    }
    env->SetLongField(thiz, GPUSurface_nativeSurface, (jlong) surface);
}

JNIEXPORT GPUSurface* getGPUSurface(JNIEnv* env, jobject thiz) {
    auto v = env->GetLongField(thiz, GPUSurface_nativeSurface);
    return reinterpret_cast<GPUSurface*>(v);
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaSurface_updateSize(JNIEnv *env, jobject instance) {
    auto surface = getGPUSurface(env, instance);
    if (surface == nullptr) {
        return;
    }
    surface->updateSize();
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaSurface_nativeInit(JNIEnv *env, jclass type) {
    jclass clazz = env->FindClass("com/lvpengwei/skiatext/SkiaSurface");
    GPUSurface_nativeSurface = env->GetFieldID(clazz, "nativeSurface", "J");
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaSurface_present(JNIEnv* env, jobject thiz) {
    auto surface = getGPUSurface(env, thiz);
    if (surface == nullptr) {
        return;
    }
    surface->present();
    LOGI("present");
}

JNIEXPORT void Java_com_lvpengwei_skiatext_SkiaSurface_release(JNIEnv* env, jobject thiz) {
    setGPUSurface(env, thiz, nullptr);
}

JNIEXPORT jlong Java_com_lvpengwei_skiatext_SkiaSurface_setupFromSurface(JNIEnv* env,
                                                                         jobject thiz,
                                                                         jobject surface) {
    auto nativeWindow = ANativeWindow_fromSurface(env, surface);
    auto gpuSurface = new GPUSurface(nativeWindow);
    setGPUSurface(env, thiz, gpuSurface);
    return reinterpret_cast<jlong>(gpuSurface);
}

}
