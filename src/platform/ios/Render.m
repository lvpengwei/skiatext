//
//  Render.m
//  Pods-SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/21.
//

#import "Render.h"
#import "RenderImpl.h"

@interface Surface ()

@property(nonatomic, strong) SurfaceImpl* surface;

@end

@implementation Render {
    RenderImpl* render;
    Surface* _surface;
}

- (instancetype)init {
    if (self = [super init]) {
        render = [[RenderImpl alloc] init];
        _surface = nil;
    }
    return self;
}

- (void)setSurface:(Surface*)surface {
    _surface = surface;
    if (surface != nil) {
        [render setSurface:[surface surface]];
    } else {
        [render setSurface:nil];
    }
}

- (void)draw {
    [render draw];
}

- (BOOL)flush {
    return [render flush];
}

@end
