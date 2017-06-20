//
//  CameraController.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 4/10/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "CameraController.hpp"
#include "GLCamera.hpp"

CameraController::CameraController(GLCamera * const targetCamera) : _targetCamera(std::shared_ptr<GLCamera>(targetCamera)) {
    
    // Adding camera update states
    {
        _cameraUpdateConditions.insert(std::pair<int, bool>(NSUpArrowFunctionKey, false));
        _cameraUpdateConditions.insert(std::pair<int, bool>(NSDownArrowFunctionKey, false));
        _cameraUpdateConditions.insert(std::pair<int, bool>(NSLeftArrowFunctionKey, false));
        _cameraUpdateConditions.insert(std::pair<int, bool>(NSRightArrowFunctionKey, false));
    }
    // Adding camera update state's keys and functions
    {
        _cameraUpdateFunctions.insert(std::pair<int, std::function<void(double)>>
                                       (NSUpArrowFunctionKey, [=](double deltaTime){
            
            glm::vec3 translated = _targetCamera->direction() * static_cast<float>(deltaTime) * speed;
            _targetCamera->translate(translated);
            
        }));
        _cameraUpdateFunctions.insert(std::pair<int, std::function<void(double)>>
                                       (NSDownArrowFunctionKey, [=](double deltaTime){
            
            glm::vec3 translated = _targetCamera->direction() * static_cast<float>(deltaTime) * speed;
            _targetCamera->translate(-translated);
            
        }));
        _cameraUpdateFunctions.insert(std::pair<int, std::function<void(double)>>
                                       (NSLeftArrowFunctionKey, [=](double deltaTime){
            
            glm::vec3 translated = _targetCamera->right() * static_cast<float>(deltaTime) * speed;
            _targetCamera->translate(translated);
            
        }));
        _cameraUpdateFunctions.insert(std::pair<int, std::function<void(double)>>
                                       (NSRightArrowFunctionKey, [=](double deltaTime){
            
            glm::vec3 translated = _targetCamera->right() * static_cast<float>(deltaTime) * speed;
            _targetCamera->translate(-translated);
            
        }));
    }
    
}


std::shared_ptr<GLCamera> CameraController::getTargetCamera() const {
    return _targetCamera;
}


CGRect CameraController::viewRect() {
    return delegate->viewRect();
}

void CameraController::updateAnimations(double deltaTime) {
    for (auto key: _cameraUpdateConditions) {
        if(_cameraUpdateConditions[key.first]) {
            _cameraUpdateFunctions[key.first](deltaTime);
        }
    }
}

std::tuple<bool, int> CameraController::canProcessKeyEvent(NSEvent *event) {
    std::tuple<bool, int> result = std::make_tuple(false, NO_KEY);
    
    NSString *keyChars = [event charactersIgnoringModifiers];
    if ([keyChars length] != 0
        && _cameraUpdateConditions.find([keyChars characterAtIndex:0]) != _cameraUpdateConditions.end()) {
        std::get<0>(result) = true;
        std::get<1>(result) = [keyChars characterAtIndex:0];
    }
    
    return result;
}


#pragma mark - IInputEventHandler
void CameraController::keyUp(NSEvent *event) {
    std::tuple<bool, int> canProcessResult = this->canProcessKeyEvent(event);
    bool cameraCanReactOnEvent = std::get<0>(canProcessResult);
    
    if (cameraCanReactOnEvent) {
        _cameraUpdateConditions[std::get<1>(canProcessResult)] = false;
    }
}

void CameraController::keyDown(NSEvent *event) {
    std::tuple<bool, int> canProcessResult = this->canProcessKeyEvent(event);
    bool cameraCanReactOnEvent = std::get<0>(canProcessResult);
    
    if (cameraCanReactOnEvent) {
        _cameraUpdateConditions[std::get<1>(canProcessResult)] = true;
    }
}
