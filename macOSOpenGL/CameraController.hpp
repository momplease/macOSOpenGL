//
//  CameraController.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef CameraController_hpp
#define CameraController_hpp

#define NO_KEY -1

#import <Cocoa/Cocoa.h>
#include "IInputEventHandler.hpp"
#include "ISceneBehaviour.hpp"
#include "GLCamera.hpp"
#include <map>
#include <tuple>


class GLCamera;

namespace {
static float speed = 10.0f;
}

struct CameraControllerDelegate {
    virtual CGRect viewRect() = 0;
};


class CameraController final: public GLCameraDelegate, public IInputEventHandler, public ISceneBehaviour {
public:
    CameraController(GLCamera * const targetCamera);
    virtual ~CameraController() = default;
    
    CameraController(const CameraController&) = delete;
    CameraController& operator=(const CameraController&) = delete;
    CameraController(CameraController&&) = delete;
    CameraController& operator=(CameraController&&) = delete;
    
    std::shared_ptr<GLCamera> getTargetCamera() const;
    
    // IInputEventHandler
    void keyUp(NSEvent *event) override;
    void keyDown(NSEvent *event) override;
    
    // ISceneBehaviour
    void updateAnimations(double deltaTime) override;
    
    // GLCameraDelegate
    CGRect viewRect() override;
    
    CameraControllerDelegate *delegate;
    
private:
    
    std::tuple<bool, int> canProcessKeyEvent(NSEvent *event);
    
    const std::shared_ptr<GLCamera> _targetCamera;
    
    // Make it as a class? What it should be?
    std::map<int, bool> _cameraUpdateConditions; // ??? better naming
    std::map<int, std::function<void(double)>> _cameraUpdateFunctions;
    ////////////////////////////////
};

#endif /* CameraController_hpp */
