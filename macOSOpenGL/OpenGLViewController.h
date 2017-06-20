//
//  OpenGLViewController.h
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/31/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OpenGLView.hpp"
#import "RenderingEngineDelegate.mm"

@interface OpenGLViewController : NSViewController <OpenGLViewDelegate,
                                                    RenderingEngineDelegate>

-(instancetype)initWithWindow:(NSWindow *)window;

@end
