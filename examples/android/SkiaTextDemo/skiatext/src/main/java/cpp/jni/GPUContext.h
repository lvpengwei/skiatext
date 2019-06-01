#ifndef ANDROID_GPUCONTEXT_H
#define ANDROID_GPUCONTEXT_H

#include <skia.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace skiatext {

    class GPUContext {
    public:
        GPUContext(EGLDisplay eglDisplay, EGLContext eglContext, GrContext* grContext);

        ~GPUContext();

        void checkError();

        GrContext* grContext;
        EGLContext eglContext;
        EGLDisplay eglDisplay;
    };

    class GPUFactory {
    public:
        GPUFactory();

        ~GPUFactory();

        static std::unique_ptr<GPUContext> FromSurface(EGLSurface eglSurface) {
            return factory.fromSurface(eglSurface);
        }

        static EGLDisplay GetEGLDisplay() {
            return factory.eglDisplay;
        }

        static EGLConfig GetEGLConfig() {
            return factory.eglConfig;
        }

        static int GetStencilBits() {
            return factory.stencilBits;
        }

        static int GetSampleCount() {
            return factory.sampleCount;
        }

    private:
        static GPUFactory factory;
        EGLDisplay eglDisplay = nullptr;
        EGLConfig eglConfig = nullptr;
        int stencilBits = 0;
        int sampleCount = 0;
        const GrGLInterface* glInterface = nullptr;

        std::unique_ptr<GPUContext> fromSurface(EGLSurface eglSurface);
    };
}

#endif //ANDROID_GPUCONTEXT_H
