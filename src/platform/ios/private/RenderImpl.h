//
//  RenderImpl.h
//  Pods-SkiaTextDemo
//
//  Created by lvpengwei on 2019/5/21.
//
#import "SurfaceImpl.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface RenderImpl : NSObject

- (void)setSurface:(SurfaceImpl * _Nullable)surface;

- (void)draw;

- (BOOL)flush;

@end

NS_ASSUME_NONNULL_END
