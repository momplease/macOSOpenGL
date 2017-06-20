//
//  AppDelegate.m
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/22/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "AppDelegate.h"
#import "OpenGLView.hpp"
#import "macOSOpenGLWindowController.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property (strong) macOSOpenGLWindowController *windowController;

@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    self.windowController = [[macOSOpenGLWindowController alloc] initWithWindow:self.window];
    self.window.windowController = self.windowController;
    [self.window becomeKeyWindow];    
    
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


@end
