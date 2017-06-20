//
//  OpenGLView.m
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/22/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "OpenGLView.hpp"
#import <OpenGL/gl.h>
#include "OpenGLShaderLoader.hpp"
#include "OpenGLTextureLoader.hpp"

@interface OpenGLView(){
    
    CVDisplayLinkRef _displayLink;
    int64_t _previousHostTime;
    
    NSTrackingArea *_trackingArea;
    
}

@end


@implementation OpenGLView

-(instancetype)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    if (self) {
        NSOpenGLPixelFormatAttribute pixelFormatAttributes[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersionLegacy,
            NSOpenGLPFAColorSize    , 24                           ,
            NSOpenGLPFAAlphaSize    , 8                            ,
            NSOpenGLPFADepthSize    , 128                          ,
            //NSOpenGLPFADoubleBuffer ,
            NSOpenGLPFAAccelerated ,
            0
        };
        
        self.pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:pixelFormatAttributes];
        //self.openGLContext = [[NSOpenGLContext alloc] initWithFormat:self.pixelFormat shareContext:nil];
        
        self.wantsLayer = YES;
        self.wantsBestResolutionOpenGLSurface = YES;
        //[self setWantsExtendedDynamicRangeOpenGLSurface:YES];
    }
    return self;
}


- (BOOL)becomeFirstResponder {
    return YES;
}

- (BOOL)acceptsFirstResponder {
    return YES;
}

#pragma mark - Display link part

// C function
static CVReturn displayLinkUpdateCallback(CVDisplayLinkRef CV_NONNULL displayLink,
                  const CVTimeStamp * CV_NONNULL inNow,
                  const CVTimeStamp * CV_NONNULL inOutputTime,
                  CVOptionFlags flagsIn,
                  CVOptionFlags * CV_NONNULL flagsOut,
                  void * CV_NULLABLE displayLinkContext) {
    @autoreleasepool {
        return [(__bridge OpenGLView*)displayLinkContext displayLinkUpdate:inOutputTime];
    }
}


- (CVReturn)displayLinkUpdate:(const CVTimeStamp *)outputTime {
    // Computing the time delta
    double deltaTime = [self calculateTimeDeltaBetween:outputTime->hostTime
                                   andPreviousHostTime:_previousHostTime];
    
    // Calling main draw
    [self.delegate viewWillUpdate:deltaTime];
    
    // Saving the last host time
    _previousHostTime = outputTime->hostTime;
    return kCVReturnSuccess;
}

- (double)calculateTimeDeltaBetween:(int64_t)currentHostTime andPreviousHostTime:(int64_t)previousHostTime {
    return ((double)(currentHostTime - previousHostTime) / CVGetHostClockFrequency());
}

- (void)prepareOpenGL {
    [super prepareOpenGL];
    
    CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
    CVDisplayLinkSetOutputCallback(_displayLink, &displayLinkUpdateCallback, (__bridge void *)self);
    
    CVDisplayLinkStart(_displayLink);
    _previousHostTime = CVGetCurrentHostTime(); // Might be updated ???

}

- (void)drawRect:(NSRect)dirtyRect {
    //[super drawRect:dirtyRect];
}


- (void)dealloc {
    CVDisplayLinkRelease(_displayLink);
}

- (void)updateTrackingAreas {
    if(_trackingArea != nil) {
        [self removeTrackingArea:_trackingArea];
    }
    
    int trackingOptions = (NSTrackingMouseMoved | NSTrackingActiveAlways);
    _trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                                                 options:trackingOptions
                                                   owner:self
                                                userInfo:nil];
    [self addTrackingArea:_trackingArea];
    
}

@end
