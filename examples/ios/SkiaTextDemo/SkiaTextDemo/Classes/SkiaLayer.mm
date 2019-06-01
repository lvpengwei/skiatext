//
//  SkiaLayer.m
//  SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/19.
//  Copyright © 2019 lvpengwei. All rights reserved.
//

#import "SkiaLayer.h"
#import <GrContext.h>
#import <gl/GrGLInterface.h>
#import <SkCanvas.h>
#import <SkGraphics.h>
#import <SkSurface.h>
#import <SkString.h>
#import <OpenGLES/ES2/gl.h>

@interface SkiaLayer () {
    GrContext *_context;
    SkCanvas *_canvas;
    EAGLContext *_eaglContext;
    GLuint framebuffer;
    GLuint colorRenderbuffer;
    GLuint depthRenderbuffer;
    int count;
}
@end
@implementation SkiaLayer
- (instancetype)init {
    self = [super init];
    if (self) {
        self.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
         [NSNumber numberWithBool:NO],
         kEAGLDrawablePropertyRetainedBacking,  //是否使用保留背景, 设置NO, 不保留, 这段代码是告诉Core Animation 层不要保留以前绘制的任何图像, 需要绘图时重新绘制整个层的内容
         kEAGLColorFormatRGBA8,
         kEAGLDrawablePropertyColorFormat,  //设置颜色值保存的位数, 8位
         nil];
        _eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
    return self;
}
- (void)dealloc {
    [self releaseSurface];
}
- (void)releaseSurface {
    if (_context == NULL) return;
    [EAGLContext setCurrentContext:_eaglContext];
    if (framebuffer) {
        glDeleteFramebuffers(1, &framebuffer);
        framebuffer = 0;
    }
    if (colorRenderbuffer) {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
    if (depthRenderbuffer) {
        glDeleteRenderbuffers(1, &depthRenderbuffer);
        depthRenderbuffer = 0;
    }
    // Free up all gpu resources in case that we get squares when rendering texts.
    _context->freeGpuResources();
    [EAGLContext setCurrentContext:nil];
}
- (void)createSurface {
    if (_eaglContext == nil) return;
    if (self.bounds.size.width == 0 || self.bounds.size.height == 0) return;
    if (_context != NULL && _canvas != NULL) return;
    [EAGLContext setCurrentContext:_eaglContext];
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glGenRenderbuffers(1, &colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    [_eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:self];
    
    GLint width = self.bounds.size.width;
    GLint height = self.bounds.size.height;
    
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    glViewport(0, 0, width, height);
    glClearStencil(0);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glStencilMask(0xffffffff);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        printf("无法使完整的framebuffer对象");
        exit(1);
    }
    const GrGLInterface * interface = GrGLCreateNativeInterface();
    _context = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)interface);
    if (NULL == interface || NULL == _context) {
        printf("Failed to initialize GL.");
        exit(1);
    }
    
    GrGLint buffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
    
    GrBackendRenderTargetDesc desc;
    desc.fWidth = SkScalarRoundToInt(width);
    desc.fHeight = SkScalarRoundToInt(height);
    desc.fConfig = kSkia8888_GrPixelConfig;
    desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
    desc.fSampleCnt = 0;
    desc.fStencilBits = 8;
    desc.fRenderTargetHandle = buffer;
    
    SkSurface * surface = SkSurface::MakeFromBackendRenderTarget(_context, desc, nullptr).release();
    _canvas = surface->getCanvas();
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    [EAGLContext setCurrentContext:nil];
}
- (void)draw {
    [self createSurface];
    if (_canvas == NULL) return;
    [EAGLContext setCurrentContext:_eaglContext];
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    
    _canvas->drawColor(SK_ColorCYAN);
    SkPaint p;
    p.setTextSize(40);
    p.setStrokeWidth(2.0);
    p.setAntiAlias(false);
    NSString *text = [NSString stringWithFormat:@"finished%d", count];
    _canvas->drawText([text UTF8String], text.length, 0, 30, p);
    SkRect r = {50, 50, 80, 80};
    p.setColor(SK_ColorLTGRAY);
    _canvas->drawRect(r, p);
    
    _context->flush();
    [_eaglContext presentRenderbuffer:GL_RENDERBUFFER];
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    [EAGLContext setCurrentContext:nil];
    count++;
}
@end
