#import "Surface.h"
#import "SurfaceImpl.h"

@interface Surface ()

@property(nonatomic, strong) SurfaceImpl* surface;

@end

@implementation Surface

+ (Surface*)FromLayer:(CAEAGLLayer*)layer {
    SurfaceImpl* surface = [SurfaceImpl FromLayer:layer];
    if (surface == nil) {
        return nil;
    }
    Surface* sur = [[Surface alloc] init];
    sur.surface = surface;
    return sur;
}

- (void)updateSize {
    [_surface updateSize];
}

- (int)width {
    return [_surface width];
}

- (int)height {
    return [_surface height];
}

- (BOOL)present {
    return [_surface present];
}

@end
