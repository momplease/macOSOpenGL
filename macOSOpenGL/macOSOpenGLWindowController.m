//
//  macOSOpenGLWindowController.m
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/31/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "macOSOpenGLWindowController.h"

@interface macOSOpenGLWindowController ()

@end

@implementation macOSOpenGLWindowController

-(instancetype)initWithWindow:(NSWindow *)window {
    self = [super initWithWindow:window];
    if (self) {
        self.openGLViewController = [[OpenGLViewController alloc] initWithWindow:window];
        self.window.contentViewController = self.openGLViewController;
    }
    return self;
}


- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

@end
