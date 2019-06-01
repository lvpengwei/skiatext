#import "GPUSurface.h"
#import <OpenGLES/ES2/gl.h>

GPUSurface::GPUSurface(CAEAGLLayer* layer) {
    this->eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    this->layer = layer;
}

GPUSurface::~GPUSurface() {
    tearDownSurface();
}

void GPUSurface::tearDownSurface() {
    if (grContext == nullptr) {
        return;
    }
    [EAGLContext setCurrentContext:eaglContext];
    if (framebuffer) {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }
    if (depthStencilRenderbuffer) {
        glDeleteRenderbuffers(1, &depthStencilRenderbuffer);
        depthStencilRenderbuffer = 0;
    }
    if (colorBuffer) {
        glDeleteRenderbuffers(1, &colorBuffer);
        colorBuffer = 0;
    }
    grContext->freeGpuResources();
    [EAGLContext setCurrentContext:nil];
}

void GPUSurface::onEnter() {
    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
}

void GPUSurface::onExit() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    [EAGLContext setCurrentContext:nil];
}

void GPUSurface::onPresent() {
    if (grContext == nullptr) {
        return;
    }
    [EAGLContext setCurrentContext:eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
    grContext->flush();
    [eaglContext presentRenderbuffer:GL_RENDERBUFFER];
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    [EAGLContext setCurrentContext:nil];
}

void GPUSurface::onUpdateSize() {
    tearDownSurface();
    CGFloat width;
    CGFloat height;
    width = layer.bounds.size.width * layer.contentsScale;
    height = layer.bounds.size.height * layer.contentsScale;

    _width = static_cast<int>(ceil(width));
    _height = static_cast<int>(ceil(height));
}

SkSurface* GPUSurface::onCreateSurface() {
    if (_width <= 0 || _height <= 0) {
        return nullptr;
    }
    SkSurface* surface = nullptr;
    [EAGLContext setCurrentContext:eaglContext];
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glGenRenderbuffers(1, &colorBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);
    [eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    
    glGenRenderbuffers(1, &depthStencilRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencilRenderbuffer);
    
    glViewport(0, 0, _width, _height);
    glClearStencil(0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glStencilMask(0xffffffff);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("无法使完整的framebuffer对象");
        ::exit(1);
    }
    
    const GrGLInterface * interface = GrGLCreateNativeInterface();
    grContext = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)interface);
    
    GrGLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);

    GrBackendRenderTargetDesc desc;
    desc.fWidth = _width;
    desc.fHeight = _height;
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    desc.fSampleCnt = 0;
    desc.fStencilBits = 8;
    desc.fRenderTargetHandle = buffer;
    surface = SkSurface::MakeFromBackendRenderTarget(grContext, desc, nullptr).release();
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    [EAGLContext setCurrentContext:nil];
    return surface;
}
