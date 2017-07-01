//
//  OpenGLViewController.m
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/31/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#import "OpenGLViewController.h"
#import "OpenGLView.hpp"
#import "OpenGLShaderLoader.hpp"
#import "OpenGLTextureLoader.hpp"

#include "GLRenderingEngine.hpp"
#include "GLScene.hpp"
#include "IInputEventHandler.hpp"

#include <memory>

@interface OpenGLViewController (){
    
    std::unique_ptr<IRenderingEngine> renderingEngine;
    std::unique_ptr<GLScene> scene;
    IInputEventHandler *inputHandler;
    
    BOOL _shoudTrackMouseDragInView;
    NSPoint _lastMouseLocation;
}

@property (weak) NSWindow *parentWindow;

@end

@implementation OpenGLViewController

-(instancetype)initWithWindow:(NSWindow *)window {
    self = [super init];
    if (self) {
        self.parentWindow = window;
    }
    return self;
}

-(void)prepareRenderer {
    GLRenderingEngine *newEngine = new GLRenderingEngine();
    GLScene *newScene = new GLScene();
    
    inputHandler = newScene;
    
    newEngine->delegate = (__bridge RenderingEngineDelegate *)self;
    newScene->delegate = newEngine;
    
    renderingEngine.reset(newEngine);
    scene.reset(newScene);
    
    renderingEngine->initialize(scene.get());
    /*try {
        
    } catch (const std::exception& e) {
        renderingEngine = nullptr;
        [self showAlertWithMessageAndTerminate:[NSString stringWithUTF8String:e.what()]];
    }*/
}


-(void)loadView {
    OpenGLView *mainView = [[OpenGLView alloc] initWithFrame:self.parentWindow.frame];
    mainView.delegate = self;
    self.view = mainView;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // Do view setup here.
    
    [self prepareRenderer];
    
    _shoudTrackMouseDragInView = NO;
    
}


#pragma mark - OpenGLViewDelegate

- (void)viewWillUpdate:(double)deltaTime {
    if (renderingEngine)
        renderingEngine->render(deltaTime);
}

#pragma mark - RenderingEngineDelegate

- (CGRect)viewRect {
    return self.view.bounds;
}

-(NSOpenGLContext *)openGLContext {
    return [((OpenGLView *)self.view) openGLContext];
}




#pragma mark - Mouse events

- (void)mouseDown:(NSEvent *)event {
    _shoudTrackMouseDragInView = YES;
}

- (void)mouseUp:(NSEvent *)event {
    _shoudTrackMouseDragInView = NO;
}


- (void)mouseDragged:(NSEvent *)event {
    
    // temporary
    if (_shoudTrackMouseDragInView && NSPointInRect(event.locationInWindow, self.view.bounds)) {
     
        NSPoint currentMouseLocation = event.locationInWindow;
     
        float xOffset = currentMouseLocation.x - _lastMouseLocation.x;
        float yOffset = currentMouseLocation.y - _lastMouseLocation.y;
     
        xOffset *= kMouseSpeed;
        yOffset *= kMouseSpeed;
     
        scene->getCamera()->updateYawBy(-xOffset);
        scene->getCamera()->updatePitchBy(-yOffset);
     
        _lastMouseLocation = currentMouseLocation;
     }
    
}


- (void)mouseMoved:(NSEvent *)event {
    _lastMouseLocation = event.locationInWindow;
}


- (void)scrollWheel:(NSEvent *)event {
    //_camera->updateFov(event.scrollingDeltaY);
}


#pragma mark - Key events

- (void)keyDown:(NSEvent *)event {
    inputHandler->keyDown(event);
 }

- (void)keyUp:(NSEvent *)event {
    inputHandler->keyUp(event);
}

- (void)showAlertWithMessage:(NSString *)message completion:(void (^)(NSModalResponse))completion {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:message];
    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert setAlertStyle:NSAlertStyleWarning];
    [alert beginSheetModalForWindow:self.parentWindow
                  completionHandler:completion];
}

@end
