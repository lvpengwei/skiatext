#import <Foundation/Foundation.h>

@interface SurfaceImpl : NSObject

+ (SurfaceImpl*)FromLayer:(CAEAGLLayer*)layer;

- (void)updateSize;

- (int)width;

- (int)height;

- (BOOL)present;

@end
