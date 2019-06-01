//
//  SkiaView.m
//  SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/18.
//  Copyright © 2019 lvpengwei. All rights reserved.
//

#import "SkiaView.h"

#import <GrContext.h>
#import <gl/GrGLInterface.h>
#import <SkCanvas.h>
#import <SkGraphics.h>
#import <SkSurface.h>
#import <SkString.h>
#import <OpenGLES/ES2/gl.h>

@interface SkiaView () <GLKViewDelegate> {
    GrContext *_context;
    SkCanvas *_canvas;
}
@end
@implementation SkiaView
- (void)dealloc {
    self.delegate = nil;
}
- (instancetype)initWithFrame:(CGRect)frame context:(EAGLContext *)context {
    self = [super initWithFrame:frame context:context];
    if (self) {
        SkGraphics::Init();
        self.drawableDepthFormat = GLKViewDrawableDepthFormat24;
        self.delegate = self;
    }
    return self;
}
- (void)glkView:(nonnull GLKView *)view drawInRect:(CGRect)rect {
    if (_context == NULL || _canvas == NULL) {
        const GrGLInterface * interface = GrGLCreateNativeInterface();
        _context = GrContext::Create(kOpenGL_GrBackend, (GrBackendContext)interface);
        if (NULL == interface || NULL == _context) {
            printf("Failed to initialize GL.");
            exit(1);
        }
        
        GrGLint buffer;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &buffer);
        
        GrBackendRenderTargetDesc desc;
        desc.fWidth = SkScalarRoundToInt(view.bounds.size.width);
        desc.fHeight = SkScalarRoundToInt(view.bounds.size.height);
        desc.fConfig = kSkia8888_GrPixelConfig;
        desc.fOrigin = kBottomLeft_GrSurfaceOrigin;
        desc.fSampleCnt = 0;
        desc.fStencilBits = 8;
        desc.fRenderTargetHandle = buffer;
        
        SkSurface * surface = SkSurface::MakeFromBackendRenderTarget(_context, desc, nullptr).release();
        _canvas = surface->getCanvas();
    }
    
    _canvas->drawColor(SK_ColorCYAN);
    SkPaint p;
    p.setTextSize(40);
    p.setStrokeWidth(2.0);
    p.setAntiAlias(true);
    _canvas->drawText("finished", 8, 50, 30, p);
    SkRect r = {50, 50, 80, 80};
    p.setColor(SK_ColorLTGRAY);
    _canvas->drawRect(r, p);
    
    _context->flush();
}
@end
