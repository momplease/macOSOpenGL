//
//  OpenGLView.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/22/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol OpenGLViewDelegate <NSObject>
@required
// Prototyping

// Here renderer will render a frame
- (void)viewWillUpdate:(double)deltaTime;
@end



@interface OpenGLView : NSOpenGLView
@property (assign) id<OpenGLViewDelegate> delegate;
@end
