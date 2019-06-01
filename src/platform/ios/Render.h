//
//  Render.h
//  Pods-SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/21.
//

#import <Foundation/Foundation.h>
#import "Surface.h"

NS_ASSUME_NONNULL_BEGIN

@interface Render : NSObject

- (void)setSurface:(Surface*)surface;

- (void)draw;

- (BOOL)flush;

@end

NS_ASSUME_NONNULL_END
