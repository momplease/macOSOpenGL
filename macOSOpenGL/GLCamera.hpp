//
//  GLCamera.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 3/27/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef GLCamera_hpp
#define GLCamera_hpp

#include <CoreGraphics/CoreGraphics.h>
#include "glm/glm.hpp"


struct GLCameraDelegate {
    virtual CGRect viewRect() = 0;
};

namespace {
static float kMouseSpeed = 0.005f;
}
    
class GLCamera {
public:
    GLCamera();
    ~GLCamera() = default;
    
    glm::mat4 view();
    glm::mat4 projection();
    glm::mat4 viewProjection();
    
    glm::vec3 direction();
    glm::vec3 right();
    glm::vec3 up();
    
    glm::vec3 position();
    void setPosition(glm::vec3 newPosition);
    void translate(glm::vec3 vector);
    
    void updateYawBy(float offset);
    void updatePitchBy(float offset);
    void updateFov(float value);
    
    struct GLCameraDelegate *delegate;
    
private:
    glm::vec3 _position;
    
    glm::vec3 _direction;
    glm::vec3 _right;
    glm::vec3 _up;
    
    float _pitch;
    float _yaw;
    float _roll;
    
    float _fov;
};



#endif /* GLCamera_hpp */
