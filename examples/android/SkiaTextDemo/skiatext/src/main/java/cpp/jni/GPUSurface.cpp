#include "GPUSurface.h"

namespace skiatext {
    GPUSurface::GPUSurface(ANativeWindow* nativeWindow, bool useMSAA) :
            nativeWindow(nativeWindow), useMSAA(useMSAA) {
        onUpdateSize();
    }

    GPUSurface::~GPUSurface() {
        tearDownSurface();
        ANativeWindow_release(nativeWindow);
        delete context;
    }

    void GPUSurface::tearDownSurface() {
        if (context != nullptr) {
            eglMakeCurrent(context->eglDisplay, eglSurface, eglSurface, context->eglContext);
            // Free up all gpu resources in case that we get squares when rendering texts next time.
            context->grContext->freeGpuResources();
            eglMakeCurrent(context->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        }
        if (eglSurface != nullptr) {
            auto eglDisplay = GPUFactory::GetEGLDisplay();
            eglDestroySurface(eglDisplay, eglSurface);
            eglSurface = nullptr;
        }
    }

    void GPUSurface::onUpdateSize() {
        tearDownSurface();
        _width = ANativeWindow_getWidth(nativeWindow);
        _height = ANativeWindow_getHeight(nativeWindow);
        auto eglConfig = GPUFactory::GetEGLConfig();
        auto eglDisplay = GPUFactory::GetEGLDisplay();
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, nativeWindow, nullptr);
    }

    SkSurface* GPUSurface::onCreateSurface() {
        if (_width <= 0 || _height <= 0) {
            return nullptr;
        }
        if (!context) {
            context = GPUFactory::FromSurface(eglSurface).release();
            if (!context) {
                return nullptr;
            }
        }

        auto stencilBits = GPUFactory::GetStencilBits();
        // Enabling MSAA will cause some rendering issues when drawing content onto a SurfaceTexture
        // from java opengl texture.
        auto sampleCount = useMSAA ? GPUFactory::GetSampleCount() : 0;
        eglMakeCurrent(context->eglDisplay, eglSurface, eglSurface, context->eglContext);
        context->checkError();

        glViewport(0, 0, _width, _height);
        glClearStencil(0);
        glClearColor(0, 0, 0, 0);
        glStencilMask(0xffffffff);
        glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLint frameBuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frameBuffer);

        GrBackendRenderTargetDesc desc;
        desc.fWidth = _width;
        desc.fHeight = _height;
        desc.fConfig = kSkia8888_GrPixelConfig;
        desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
        desc.fSampleCnt = sampleCount;
        desc.fStencilBits = stencilBits;
        desc.fRenderTargetHandle = frameBuffer;
        auto surface = SkSurface::MakeFromBackendRenderTarget(context->grContext,
                                                              desc, nullptr).release();
        return surface;
    }

    void GPUSurface::onEnter() {
        if (!context) {
            return;
        }
        eglMakeCurrent(context->eglDisplay, eglSurface, eglSurface, context->eglContext);
        context->checkError();
    }

    void GPUSurface::onExit() {
        if (!context) {
            return;
        }
        eglMakeCurrent(context->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        context->checkError();
    }

    void GPUSurface::onPresent() {
        if (!context) {
            return;
        }
        eglMakeCurrent(context->eglDisplay, eglSurface, eglSurface, context->eglContext);
        context->checkError();
        context->grContext->flush();
        eglSwapBuffers(context->eglDisplay, eglSurface);
        context->checkError();
        eglMakeCurrent(context->eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

}