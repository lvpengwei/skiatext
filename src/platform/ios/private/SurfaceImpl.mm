#import "SurfaceImpl.h"
#import "GPUSurface.h"

@interface SurfaceImpl ()

@property(nonatomic) skiatext::Surface* surface;

@end

@implementation SurfaceImpl {

}

+ (SurfaceImpl*)FromLayer:(CAEAGLLayer*)layer {
    if (layer == nil) {
        return nil;
    }
    auto surface = new GPUSurface(layer);
    SurfaceImpl* pagSurface = [[SurfaceImpl alloc] initWidthSurface:surface];
    return pagSurface;
}

- (instancetype)initWidthSurface:(skiatext::Surface*)value {
    if (self = [super init]) {
        _surface = value;
    }
    return self;
}

- (void)dealloc {
    delete _surface;
}

- (void)updateSize {
    _surface->updateSize();
}

- (int)width {
    return _surface->width();
}

- (int)height {
    return _surface->height();
}

- (BOOL)present {
    return _surface->present();
}

@end
