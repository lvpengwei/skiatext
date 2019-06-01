//
//  SkiaTextLayer.m
//  Pods-SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/21.
//
#import "Render.h"
#import "SkiaTextLayer.h"
#import <UIKit/UIKit.h>

@implementation SkiaTextLayer {
    Render* _renderer;
    Surface* _surface;
}

- (instancetype)init {
    if (self = [super init]) {
        self.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithBool:NO],
                                   kEAGLDrawablePropertyRetainedBacking,  //是否使用保留背景, 设置NO, 不保留, 这段代码是告诉Core Animation 层不要保留以前绘制的任何图像, 需要绘图时重新绘制整个层的内容
                                   kEAGLColorFormatRGBA8,
                                   kEAGLDrawablePropertyColorFormat,  //设置颜色值保存的位数, 8位
                                   nil];
        self.contentsScale = [UIScreen mainScreen].scale;
        _renderer = [[Render alloc] init];
        _surface = [Surface FromLayer:self];
        [_renderer setSurface:_surface];
    }
    return self;
}

- (void)flush {
    [_renderer flush];
}

- (void)layoutSublayers {
    [super layoutSublayers];
    [_surface updateSize];
}

@end
