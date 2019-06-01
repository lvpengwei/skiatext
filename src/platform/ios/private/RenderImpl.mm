//
//  RenderImpl.m
//  Pods-SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/21.
//

#import "RenderImpl.h"
#include "skiatext.h"

@interface SurfaceImpl ()

@property(nonatomic) skiatext::Surface* surface;

@end

@implementation RenderImpl {
    skiatext::Renderer* render;
}

- (instancetype)init {
    if (self = [super init]) {
        render = new skiatext::Renderer();
    }
    return self;
}

- (void)dealloc {
    delete render;
}

- (void)setSurface:(SurfaceImpl*)surface {
    skiatext::Surface* s = nullptr;
    if (surface != nil) {
        s = [surface surface];
    }
    render->setSurface(s);
}

- (void)draw {
    render->draw();
}

- (BOOL)flush {
    return render->flush();
}

@end
