#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface Surface : NSObject

+ (Surface*)FromLayer:(CAEAGLLayer*)layer;

- (void)updateSize;

- (int)width;

- (int)height;

- (BOOL)present;

@end
