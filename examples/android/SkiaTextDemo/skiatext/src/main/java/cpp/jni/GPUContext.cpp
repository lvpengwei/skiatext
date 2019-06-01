#include "GPUContext.h"
#include <android/log.h>

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"libpag",__VA_ARGS__)

namespace skiatext {

    GPUContext::GPUContext(EGLDisplay eglDisplay, EGLContext eglContext, GrContext* grContext) :
            eglDisplay(eglDisplay), eglContext(eglContext), grContext(grContext) {
    }

    GPUContext::~GPUContext() {
        grContext->releaseResourcesAndAbandonContext();
        SkSafeUnref(grContext);
        eglDestroyContext(eglDisplay, eglContext);
    }

    void GPUContext::checkError() {
        auto error = eglGetError();
        if (error == EGL_SUCCESS) {
            return;
        }
        switch (error) {
            case EGL_NOT_INITIALIZED :
                LOGE("EGL_NOT_INITIALIZED");
                break;
            case EGL_BAD_ACCESS :
                LOGE("EGL_BAD_ACCESS");
                break;
            case EGL_BAD_ALLOC :
                LOGE("EGL_BAD_ALLOC");
                break;
            case EGL_BAD_ATTRIBUTE :
                LOGE("EGL_BAD_ATTRIBUTE");
                break;
            case EGL_BAD_CONFIG :
                LOGE("EGL_BAD_CONFIG");
                break;
            case EGL_BAD_CONTEXT :
                LOGE("EGL_BAD_CONTEXT");
                break;
            case EGL_BAD_CURRENT_SURFACE :
                LOGE("EGL_BAD_CURRENT_SURFACE");
                break;
            case EGL_BAD_DISPLAY :
                LOGE("EGL_BAD_DISPLAY");
                break;
            case EGL_BAD_MATCH :
                LOGE("EGL_BAD_MATCH");
                break;
            case EGL_BAD_NATIVE_PIXMAP :
                LOGE("EGL_BAD_NATIVE_PIXMAP");
                break;
            case EGL_BAD_NATIVE_WINDOW :
                LOGE("EGL_BAD_NATIVE_WINDOW");
                break;
            case EGL_BAD_PARAMETER :
                LOGE("EGL_BAD_PARAMETER");
                break;
            case EGL_BAD_SURFACE :
                LOGE("EGL_BAD_SURFACE");
                break;
            case EGL_CONTEXT_LOST :
                LOGE("EGL_CONTEXT_LOST");
                break;
        }
    }

    GPUFactory GPUFactory::factory = {};

    GPUFactory::GPUFactory() {
        eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        EGLint majorVersion;
        EGLint minorVersion;
        eglInitialize(eglDisplay, &majorVersion, &minorVersion);
        eglBindAPI(EGL_OPENGL_ES_API);
        EGLint numConfigs = 0;
        const EGLint configAttribs[] = {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 8,
                EGL_STENCIL_SIZE, 8,
                EGL_SAMPLE_BUFFERS, 1,
                EGL_SAMPLES, 4,
                EGL_NONE
        };
        eglChooseConfig(eglDisplay, configAttribs, &eglConfig, 1, &numConfigs);
        SkASSERT(numConfigs > 0);
        eglGetConfigAttrib(eglDisplay, eglConfig, EGL_STENCIL_SIZE, &stencilBits);
        eglGetConfigAttrib(eglDisplay, eglConfig, EGL_SAMPLES, &sampleCount);

        static const EGLint kEGLContextAttribsForOpenGLES[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        EGLContext eglContext = eglCreateContext(
                eglDisplay, eglConfig, nullptr, kEGLContextAttribsForOpenGLES);
        int surfaceAttributes[] = {
                EGL_WIDTH, 1,
                EGL_HEIGHT, 1,
                EGL_NONE
        };
        auto eglSurface = eglCreatePbufferSurface(eglDisplay, eglConfig, surfaceAttributes);
        eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
        auto glVersion = glGetString(GL_VERSION);
        auto glVendor = glGetString(GL_VENDOR);
        auto glRenderer = glGetString(GL_RENDERER);
        auto glGL_EXTENSIONS = glGetString(GL_EXTENSIONS);
        GLint maxTextureSize = 0;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

        glInterface = GrGLCreateNativeInterface();
        eglDestroySurface(eglDisplay, eglSurface);
        eglDestroyContext(eglDisplay, eglContext);
    }

    GPUFactory::~GPUFactory() {
        eglTerminate(eglDisplay);
        SkSafeUnref(glInterface);
    }

    std::unique_ptr<GPUContext> GPUFactory::fromSurface(EGLSurface eglSurface) {
        static const EGLint contextAttributes[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        EGLContext eglContext = eglCreateContext(
                eglDisplay, eglConfig, nullptr, contextAttributes);
        eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

        GrContext* grContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext) glInterface);
        if (grContext == nullptr) {
            return nullptr;
        }
        auto data = new GPUContext(eglDisplay, eglContext, grContext);
        return std::unique_ptr<GPUContext>(data);
    }
}