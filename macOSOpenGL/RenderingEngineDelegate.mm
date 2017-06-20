//
//  RenderingEngineDelegate.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/1/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Cocoa/Cocoa.h>

// Rendering Engine Delegate
@protocol RenderingEngineDelegate <NSObject>
@required
- (NSOpenGLContext *)openGLContext;
- (CGRect)viewRect;
@end
